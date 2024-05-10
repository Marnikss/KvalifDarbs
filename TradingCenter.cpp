// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/TradingCenter.h"
#include <Speele/Cilveks.h>
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "UserInterface/TradingPanel.h" 
#include <Kismet/GameplayStatics.h>


// Sets default values
ATradingCenter::ATradingCenter()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    InteractionWidgetClass = nullptr;
    TradingPanelClass = nullptr;

    InteractionWidgetInstance = nullptr;

    TradingPanel = nullptr;
    
}

void ATradingCenter::BeginPlay()
{
    Super::BeginPlay();

    bPlayerInside = false; // Initialize bPlayerInside to false

    UBoxComponent* TradingCollision = FindComponentByClass<UBoxComponent>();

    if (TradingCollision)
    {
        TradingCollision->OnComponentBeginOverlap.AddDynamic(this, &ATradingCenter::OnOverlapBegin);
        TradingCollision->OnComponentEndOverlap.AddDynamic(this, &ATradingCenter::OnOverlapEnd);
    }

    // Create the widget instance
    if (InteractionWidgetClass)
    {
        InteractionWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), InteractionWidgetClass);
        if (InteractionWidgetInstance)
        {
            // Attach the widget to the trading center
            UWidgetComponent* WidgetComponent = NewObject<UWidgetComponent>(this);
            if (WidgetComponent)
            {
                WidgetComponent->SetWidget(InteractionWidgetInstance);
                WidgetComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
                WidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f)); // Adjust the position if needed
                WidgetComponent->RegisterComponent();
                WidgetComponent->SetVisibility(false); // Initially hide the widget
                UE_LOG(LogTemp, Warning, TEXT("Widget created and hidden"));
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to create WidgetComponent!"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to create widget!"));
        }

        // Initialize TradingPanel here
        TradingPanel = CreateWidget<UTradingPanel>(GetWorld(), TradingPanelClass); // Should be TradingPanelClass instead of InteractionWidgetClass
        if (TradingPanel)
        {
            UE_LOG(LogTemp, Warning, TEXT("Trading Panel created and added to viewport"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to create Trading Panel!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("InteractionWidgetClass is null!"));
    }
}

void ATradingCenter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Warning, TEXT("Overlap Begin"));
    ACilveks* Player = Cast<ACilveks>(OtherActor);
    if (Player && InteractionWidgetInstance)
    {
        bPlayerInside = true;

        Player->CurrentCenter = this;

        

        // Print a message when the player enters the trading center
        UE_LOG(LogTemp, Warning, TEXT("Player entered the trading center!"));

        // Show interaction widget
        UWidgetComponent* WidgetComponent = FindComponentByClass<UWidgetComponent>();
        if (WidgetComponent)
        {
            WidgetComponent->SetVisibility(true);
            UE_LOG(LogTemp, Warning, TEXT("Widget shown"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("WidgetComponent not found!"));
        }
    }
}



void ATradingCenter::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    UE_LOG(LogTemp, Warning, TEXT("Overlap End"));

    

    ACilveks* Player = Cast<ACilveks>(OtherActor);
    if (Player && InteractionWidgetInstance)
    {

            

        // Print a message when the player exits the trading center
        UE_LOG(LogTemp, Warning, TEXT("Player exited the trading center!"));

        bPlayerInside = false;

        // Hide interaction widget
        UWidgetComponent* WidgetComponent = FindComponentByClass<UWidgetComponent>();
        if (WidgetComponent)
        {
            WidgetComponent->SetVisibility(false);
            UE_LOG(LogTemp, Warning, TEXT("Widget hidden"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Widget component is null!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Player or InteractionWidgetInstance is null!"));
    }
}


// Called every frame
void ATradingCenter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ATradingCenter::ToggleTradingPanel()
{
    // Iegūst atsauci uz spēlētāju
    ACilveks* Player = Cast<ACilveks>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    // ja spēlētājs ir iekšpus mijiedarbības zonas
    if (bPlayerInside)
    {
        // izvada testa līniju
        UE_LOG(LogTemp, Warning, TEXT("Player in"));
        // ja pastāv atsauce uz TradingPanel logrīku
        if (TradingPanel)
        {
            // ja TradingPanel logrīks jau nerādās spēlētāja ekrānā
            if (!TradingPanel->IsInViewport())
            {
                // iegūst atsauci uz Spēlētāja kontrolieri lai varētu izmainīt tā režīmu
                APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
                if (PlayerController)
                {
                    // tiek parādīts peles kursors, tiek ļauts ar peli klikšķināt un darboties attiecīgi 
                    PlayerController->bShowMouseCursor = true;
                    PlayerController->bEnableClickEvents = true;
                    PlayerController->bEnableMouseOverEvents = true;
                    PlayerController->SetInputMode(FInputModeGameOnly());
                }
                // pievieno paneli spēlētāja ekrānam ( padara to redzamu )
                TradingPanel->AddToViewport();
                
            }
            else
            {
                // Ja panelis jau ir redzams, noņem to
                TradingPanel->RemoveFromParent(); 
                // iegūst kontroliera atsauci un ievieš atpakaļ visus parastos peles režīmus
                APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
                if (PlayerController)
                {
                    PlayerController->bShowMouseCursor = false;
                    PlayerController->bEnableClickEvents = false;
                    PlayerController->bEnableMouseOverEvents = false;
                    PlayerController->SetInputMode(FInputModeGameOnly());
                }
            }
        }
    }
    else // Ja spēlētājs nav tirdzniecības centra iekšpusē:
    {
        UE_LOG(LogTemp, Warning, TEXT("Player out"));
        // Ja tirdzniecības panelis ir redzams, noņem to
        if (TradingPanel && TradingPanel->IsInViewport())
        {
            TradingPanel->RemoveFromParent(); 
        }
    }
}
