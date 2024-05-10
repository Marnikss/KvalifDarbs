#include "SpeeleGameMode.h" // atsauce uz header failu
#include "UObject/ConstructorHelpers.h" // atsauce uz blueprint meklētāju(iekļauj funkciju)
#include "Components/DirectionalLightComponent.h" // atsauce uz Unreal engine Directionallightcomponent objekta clasi
#include <Engine/DirectionalLight.h> // atsauce uz Unreal engine DirectionalLight objekta klasi
#include "EngineUtils.h" // iegūst meklēšanas palīgfunkcijas no paša engine
#include <UserInterface/StartScreenWidget.h> // Atsauce uz startscreenwidget klasi



ASpeeleGameMode::ASpeeleGameMode() // klases konstruktors
{
    // ar constructorhelpers funkciju tiek atrasts startscreenwidget izveidotais blueprints
    static ConstructorHelpers::FClassFinder<UStartScreenWidget> StartScreenWidgetClass(TEXT("/Game/Blueprints/MAINMENU/WBP_StartScreenWidget"));
    
    if (StartScreenWidgetClass.Succeeded()) // ja ir atrasts  blueprints
    {
        StartScreenWidgetClassBP = StartScreenWidgetClass.Class; // StartScreenWidget blueprinta klases atsauce tiek saglabāta 
        // Startscreenwidgetclasspb mainīgajā
    }
}

void ASpeeleGameMode::BeginPlay() // funkcija sākas vienmēr kad spēle tiek palaista
{
    //Super::BeginPlay(); // izpilda veecāku klases beginplay fun

    // iestata taimeri, kas regulāri izpilda funkciju 
    // UpdateLightIntensity() ar norādītu laika intervālu.
    GetWorld()->GetTimerManager().SetTimer(DayNightTimerHandle, this, &ASpeeleGameMode::UpdateLightIntensity, 0.01f, true);

    // funkcija kas pievieno StartScreenWidget logrīku ekrānam
    AddStartScreenWidgetToViewport();
}

void ASpeeleGameMode::AddStartScreenWidgetToViewport()
{
    if (StartScreenWidgetClassBP) // ja blueprints ir atrasts un pastāv
    {
        // izveido jaunu logrīku, izmantojot UStartScreenWidget klasi un norādīto Blueprint klasi (StartScreenWidgetClassBP).
        StartScreenWidgetInstance = CreateWidget<UStartScreenWidget>(GetWorld(), StartScreenWidgetClassBP);
        if (StartScreenWidgetInstance) // ja logrīks ir izveidots un pastāv
        {
            // logrīku pievieno ekrānam
            StartScreenWidgetInstance->AddToViewport();
            
            // Komentārs kas apstiprina pievienošanu ekrānam, (parādās terminālī)
            UE_LOG(LogTemp, Warning, TEXT("StartScreenWidget added to viewport!"));
        }
        else
        {
            // Izvadās ja nesanāca izveidot StartScreenWidget
            UE_LOG(LogTemp, Error, TEXT("Failed to create StartScreenWidget!"));
        }
    }
    else
    {
        // izvadās, ja Netika atrasta StartScreenWidget C++ klase
        UE_LOG(LogTemp, Error, TEXT("StartScreenWidget class not found!"));
    }
}


void ASpeeleGameMode::UpdateLightIntensity() // Funkcija kas maina gaismas gaišumu
{
    //Izveido mainīgo, kas saturēs DirectionalLight objekta atsauci
    ADirectionalLight* DirectionalLight = nullptr; // tiek uzstādīts kā nullptr pēc noklusējuma
    for (TActorIterator<ADirectionalLight> It(GetWorld()); It; ++It) // tiek iziets cauri visiem spēles objektiem
        // un atrasts directionallight objekts (Unreal engine tur tas atrodās pēc noklusējuma), tas tiek saglabāts
        // mainīgajā
    {
        DirectionalLight = *It;
        break;
    }

    // veic komponenta pieprasījumu no atrastā DirectionalLight objekta, lai iegūtu tā virziena gaismas komponentu
    UDirectionalLightComponent* LightComponent = Cast<UDirectionalLightComponent>(DirectionalLight->GetLightComponent());
    if (LightComponent) // ja gaismas komponents atrasts
    {
        static float IntensityStep = 0.00005f; // float mainīgais, kas nosaka gaismas mainīšanas soļa lielumu
        static bool Increasing = true; // boolean mainīgais, kas pārbauda vai gaisma paliek tumšāka vai gaišāka

        float Intensity = LightComponent->Intensity; // float mainīgais kas iegūst pašreizējo gaismas spilgtumu
        
        // ja gaismas spilgtums palielinās
        if (Increasing)
        {
            Intensity += IntensityStep; // intensitāti palielina pa norādīto soli
            if (Intensity >= 1.0f) // ja sasniegts maksimālais gaišums
            {
                Intensity = 1.0f; // tiek iestatīta uz maksimālo vērtību
                Increasing = false; // sāk mazināties gaišums
            }
        }
        else // ja gaišums mazinās
        {
            Intensity -= IntensityStep; // gaišumu pataisa mazāku pa noteikto soli
            if (Intensity <= -1.0f) // sasniegts tumšākais punkts
            {
                Intensity = -1.0f; // iestatīta minimālā vērtība
                Increasing = true; // sāk padarīt gaismu gaišāku
            }
        }

        LightComponent->SetIntensity(Intensity); // pēc katras pārbaudes nosaka jauno intensity vērtību
        
        // Nosaka gaismas krāsas (RGBA formāta)
        LightComponent->SetLightColor(FLinearColor(0.05f, 0.05f, 0.05f, 1.0f));
    }
}

void ASpeeleGameMode::ToggleStartScreenWidget() // parādīt vai slēpt startscreenwidget blueprintu
{
    // Print līnija iekš termināļa testam
    UE_LOG(LogTemp, Warning, TEXT("ToggleStartScreenWidget() called"));

    // iegūst atsauci uz spēles pasauli (World) no pašreizējās aktīvās spēles režīma instances
    UWorld* World = GetWorld();
    if (World) // ja instance pastāv
    {
        for (TObjectIterator<UStartScreenWidget> Itr; Itr; ++Itr) // cikls iet cauri visiem 
            // objektiem un meklē StartScreenWidget
        {
            UStartScreenWidget* StartScreenWidget = *Itr; // izveidot norādi uz StartScreenwidget, kurš ir atrasts
            if (StartScreenWidget) // ja startscreenwidget pastāv
            {
                if (StartScreenWidget->IsVisible()) // ja widgets ir redzams
                {
                    StartScreenWidget->SetVisibility(ESlateVisibility::Hidden); // paslēpj widgetu

                    // Print līnija terminālī testam
                    UE_LOG(LogTemp, Warning, TEXT("StartScreenWidget visibility hidden"));

                    // Iegūst spēlētāja kontrolleri un iestata peles un ievades režīmu
                    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
                    if (PlayerController) // ja kontrolleris ir atrasts
                    {
                        // iestata spēles kontrolleri tā, lai peles rādītājs nebūtu redzams un lai 
                        // spēlētājs varētu vadīt spēli tikai ar tastatūru un peles palīdzību.
                        PlayerController->SetInputMode(FInputModeGameOnly());
                        PlayerController->bShowMouseCursor = false;
                    }
                }
                else // ja widgets nav redzams
                {
                    StartScreenWidget->SetVisibility(ESlateVisibility::Visible); // padara widgetu redzamu

                    // Print līnija termināļa testam
                    UE_LOG(LogTemp, Warning, TEXT("StartScreenWidget visibility visible"));

                    // Iegūst spēlētāja kontrolleri un iestata peles un ievades režīmu
                    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
                    if (PlayerController) // ja kontrolleris atrasts
                    {
                        PlayerController->bShowMouseCursor = true;
                        PlayerController->SetInputMode(FInputModeGameAndUI().SetWidgetToFocus(StartScreenWidget->TakeWidget()));
                    }
                }
            }
        }
    }
}


