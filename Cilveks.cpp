#include "Cilveks.h" // atsauce uz header failu
#include "UserInterface/CilvekaHUD.h" // atsauce uz CilvekaHUD failu
#include "Components/InventoryComponent.h" // atsauce uz InventoryComponent failu
#include "World/Pickup.h" // atsauce uz Pickup failu
#include "Items/TradingCenter.h" // atsauce uz TradingCenter failu
#include "Components/WidgetComponent.h" // atsauce Unreal engine pamata klases WidgetComponent failu
#include <HealthBar/HealthBarWidget.h> // atsauce uz Healtbarwidget failu
#include "C:\GAME_SSD_FAST\2\Speele\Intermediate\ProjectFiles\Consts.h" // atsauce uz Consts failu
#include "GameFramework/SpringArmComponent.h" // atsauce uz Springarmcomponent objekta pamata failu
#include "Camera/CameraComponent.h"  // atsauce uz cameracomponent objekta pamata failu
#include "GameFramework/CharacterMovementComponent.h" // atsauce uz CharacterMovementComponent objekta pamata failu
#include "DrawDebugHelpers.h" // atsauce uz zīmējošā stara pamata failu
#include "Perception/AIPerceptionStimuliSourceComponent.h" // atsauce uz stimulācijas pamata failu priekš npc
#include "Perception/AISense_Sight.h" // atsauce uz npc redzes komponenta pamata failu
#include <AI_NPC/NPC.h> // atsauce uz NPC klases failu
#include <UserInterface/StartScreenWidget.h> // atsauce uz Startscreenwidget logrīka klases failu
#include "SpeeleGameMode.h" // atsauce uz speelegamemode klasi

// cilveka konstruktors
ACilveks::ACilveks() :
    // izveido komponentu dzīvību vērtība
    WidgetComponent{CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthValue"))},
    // nosaka maksimālo dzīvību daudzumu
    Health{ MaxHealth }
{
    // izveido inventory komponentu
    InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

    // atrod izveidoto WeaponData datu tabulu
    static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTableAsset(TEXT("/Game/ItemData/WeaponData"));
    // ja tiek atrasta tabula
    if (ItemDataTableAsset.Succeeded())
    {
        // Mainīgajam tiek piešķirta atsauce uz datu tabulu
        MyItemDataTable = ItemDataTableAsset.Object;
    }

    // ja pastāv health logrīks
    if (WidgetComponent)
    {
        // pievieno logrīku spēlētāja ķermenim
        WidgetComponent->SetupAttachment(RootComponent);

        // nosaka logrīka lielumu uz ekrāna un tā atrašanās vietu
        WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
        WidgetComponent->SetRelativeLocation(defs::HealthBarZ);

        // atrod blueprintu, kurš būs pats healhbar logrīks
        static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/UI/BP_HealthBar"));
        if (WidgetClass.Succeeded())
        {
            // piešķir mainīgajam atsauci uz atrasto blueprintu
            WidgetComponent->SetWidgetClass(WidgetClass.Class);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to find health bar widget class"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to create health bar widget component"));
    }

    // tiek iespējota tick funkcija un darbību atjaunināšana 
    PrimaryActorTick.bCanEverTick = true;
    

    // nosaka spēlētāja lekšanas ātrumu lecot
    GetCharacterMovement()->JumpZVelocity = 800.0f;
    // nosaka spēlētāja maksimālo staigāšanas ātrumu 
    GetCharacterMovement()->MaxWalkSpeed = 300.0f;
    
    // nosaka mijiedarbības pārbaudes biežumu
    InteractionCheckFrequency = 0.1;
    // nosaka mijiedarbības pārbaudes distanci
    InteractionCheckDistance = 225.0f;

    // izveido jaunu objektu kas kalpos kā spēlētāja inventory
    PlayerInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("PlayerInventory"));
    // nosaka inventory vietas ietilpību
    PlayerInventory->SetSlotsCapacity(50);
    // nosaka inventory kopējo priekšmetu smaguma ietilpību
    PlayerInventory->SetWeightCapacity(50.0f);
}

// savieno nospiestās pogas ar funkcijām, kuras tām paredzētas
void ACilveks::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ACilveks::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ACilveks::MoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &ACilveks::Turn);
    PlayerInputComponent->BindAxis("LookUp", this, &ACilveks::LookUp);

    PlayerInputComponent->BindAction("ToggleMenu", IE_Pressed, this, &ACilveks::ToggleMenu);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACilveks::StartJump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACilveks::StopJump);

    PlayerInputComponent->BindAxis("WalkBackward", this, &ACilveks::WalkBackward);
    PlayerInputComponent->BindAxis("Crouch", this, &ACilveks::ToggleCrouch);

    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ACilveks::StartSprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ACilveks::StopSprint);

    PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACilveks::BeginInteract);
    PlayerInputComponent->BindAction("Interact", IE_Released, this, &ACilveks::EndInteract);

    PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ACilveks::OnAttack);
    
    PlayerInputComponent->BindAction("Pickup", IE_Pressed, this, &ACilveks::OnPickupPressed);
    PlayerInputComponent->BindAction("Pickup", IE_Released, this, &ACilveks::OnPickupReleased);

    PlayerInputComponent->BindAction("ToggleTradingPanel", IE_Pressed, this, &ACilveks::ToggleTradingPanel);

    PlayerInputComponent->BindAction("ToggleStartScreenWidget", IE_Pressed, this, &ACilveks::ToggleStartScreenWidget);

}

void ACilveks::WalkBackward(float Value)
{
    // Value būs pozitīvs, kad pārvietojas uz priekšu un negatīvs, kad pārvietojas atpakaļ
    const float AdjustedValue = -Value; // vērtība tiek aprēķināta, lai panāktu kustību atpakaļ
    if (Controller != nullptr && AdjustedValue != 0.0f)
    {
        // iegūst rotāciju
        const FRotator Rotation = Controller->GetControlRotation();
        // izveido rotāciju
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // iegūst virzienu kurā pārvietosies spēlētājs, izmantojot rotāciju
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

        // pievieno kustību noteiktajā virzienā
        AddMovementInput(Direction, AdjustedValue);
    }
}



void ACilveks::ToggleCrouch(float Value)
{
    // Ja vērtība ir lielāka par 0 un spēlētājs nekrīt, tad tiek aktivizēta pietupšanās
    if (Value > 0.0f && !GetCharacterMovement()->IsFalling())
    {
        // Positive value means the crouch action is pressed and not mid-air
        Crouch();
    }
    else
    {
        // Ja vērtība nav lielāka par 0 vai spēlētājs krīt, tad tiek veikta pāriešana uz normālo stāvokli
        UnCrouch();
    }
}


void ACilveks::OnPickupPressed()
{
    // pārbauda vai ir derīgs priekšmets un vai jau nenotiek mijiedarbība
    if (CurrentPickup && bIsInteracting == false)
    {
        // iestata mijiedarbību uz true
        bIsInteracting = true;

        UE_LOG(LogTemp, Warning, TEXT("Pickup initiated."));

        // izsauc Pickup funkciju no pickup klases, padod spēlētāju kā parametru
        CurrentPickup->Pickup(this);
        
    }
}

void ACilveks::OnPickupReleased()
{
    // iestata mijiedarbību uz false
    bIsInteracting = false;

    UE_LOG(LogTemp, Warning, TEXT("Pickup released: Interacting Flag reset."));
}

// kustība uz priekšu
void ACilveks::MoveForward(float Value)
{
    // nosaka vai spēlētājam jāvēršās kustības virzienā
    GetCharacterMovement()->bOrientRotationToMovement = (Value >= 0.0f);

    if (Controller != nullptr && Value != 0.0f)
    {
        // iegūst rotāciju
        const FRotator Rotation = Controller->GetControlRotation();
        // Izveido rotāciju, ierobežojot tikai Y ass rotāciju
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // Iegūst virzienu, kurā pārvietosies spēlētājs
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

        // Pievieno kustību šajā virzienā, ņemot vērā kustības vērtību
        AddMovementInput(Direction, Value);
    }
}

// kustības pa labi un pa kreisi
void ACilveks::MoveRight(float Value)
{
    if (Controller != nullptr && Value != 0.0f)
    {
        // iegūst rotāciju
        const FRotator Rotation = Controller->GetControlRotation();

        // Izveido rotāciju, ierobežojot tikai Y ass rotāciju
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // Iegūst virzienu, kurā pārvietosies spēlētājs
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        // Pievieno kustību šajā virzienā, ņemot vērā kustības vērtību
        AddMovementInput(Direction, Value);
    }
}

// spēlētāja pagriešanās
void ACilveks::Turn(float Value)
{
    // Pievieno ievadi, lai kontrolētu pagriezienu
    AddControllerYawInput(Value);
}


// skatīšanās uz augšu un leju
void ACilveks::LookUp(float Value)
{
    if (Controller != nullptr && Value != 0.0f)
    {
        // Pievieno ievadi, lai kontrolētu pavērsienu
        AddControllerPitchInput(Value);
    }
}

// lekšanas uzsākšana
void ACilveks::StartJump()
{
    // lekšanas poga nospiesta
    bPressedJump = true;
}
// lekšanas beigšana
void ACilveks::StopJump()
{
    // lekšanas poga atlaista
    bPressedJump = false;
}

// sprinta uzsākšana
void ACilveks::StartSprint()
{
    // šajā brīdī sprinto
    bIsSprinting = true;
    // palielina kustības ātrumu
    GetCharacterMovement()->MaxWalkSpeed *= 3.0f; 
}

void ACilveks::StopSprint()
{
    bIsSprinting = false;
    GetCharacterMovement()->MaxWalkSpeed /= 3.0f; 
}

void ACilveks::SetupStimulusSource()
{
    // Izveido UAIPerceptionStimuliSourceComponent objektu un piesaista to StimulusSource mainīgajam
    StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
    // Pārbauda, vai StimulusSource objekts pastāv
    if (StimulusSource)
    {
        // Reģistrē StimulusSource objektu uztveres sistēmā, lai tas būtu spējīgs reaģēt uz redzēšanu
        StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
        // Reģistrē StimulusSource objektu uztveres sistēmā, lai tas būtu spējīgs reaģēt uz uztveri
        StimulusSource->RegisterWithPerceptionSystem();
    }
}

// spēlētāja montāžas funkcija
void ACilveks::OnAttack()
{
    // ja cirvis ir rokā
    if (bAxeEquipped)
    {
        // ja spēlētājam piesaistīta montāža
        if (Montage)
        {
            // izspēlēt montāžu
            PlayAnimMontage(Montage);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot attack without the axe equipped."));
    }
}

// beginplay funkcija spēles sākumā
void ACilveks::BeginPlay()
{
    // izpilda noklusējuma beginplay funkciju
    Super::BeginPlay();

    // piekļūst spēlētāja HUD instancei un saglabā to mainīgajā
    HUD = Cast<ACilvekaHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

    // iegūst kameras instanci ar mainīgā palīdzību
    APlayerCameraManager* const camMan = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
    // nosaka maksimālo un minimālo skatīšanās leņķa lielumu uz augšu un uz leju
    camMan->ViewPitchMin = -70.f;
    camMan->ViewPitchMax = 20.f;

    // piesaista spēlētāja atrašanās vietu mainīgajam
    FVector PlayerPosition = GetActorLocation();

    UE_LOG(LogTemp, Warning, TEXT("Player spawned at X=%f, Y=%f, Z=%f"), PlayerPosition.X, PlayerPosition.Y, PlayerPosition.Z);
}

void ACilveks::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    // Pārbauda, vai ir pienācis laiks veikt mijiedarbības pārbaudi
    if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency)
    {
        PerformInteractionCheck();
    }
    // Pārbauda, vai ir pieejams veselības joslas logrīks un atjauno tā vērtību
    if (auto const widget = Cast<UHealthBarWidget>(WidgetComponent->GetUserWidgetObject()))
    {
        // Iestata health joslas vērtību logrīkā atbilstoši esošajai un maksimālajai vērtībai
        widget->SetBarValuePercent(Health / MaxHealth);

        // Iestata logrīka redzamību atkarībā no esošās un maksimālās veselības vērtībām
        WidgetComponent->SetVisibility((Health < MaxHealth) ? true : false);
    }
}

void ACilveks::PerformInteractionCheck()
{
    // Atjauno pēdējās mijiedarbības pārbaudes laiku
    InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

    // Nosaka spēlētāja skata virzienu mijiedarbības pārbaudes sākuma un beigu punktus
    FVector TraceStart{ GetPawnViewLocation() };
    FVector TraceEnd{ TraceStart + (GetViewRotation().Vector() * InteractionCheckDistance) };

    // Nosaka skatīšanās virzienu
    float LookDirection = FVector::DotProduct(GetActorForwardVector(), GetViewRotation().Vector());
    // ja spēlētājs skatās uz priekšu
    if (LookDirection > 0)
    {
        // uzzīmē sarkanu līniju testa mērķiem
        //DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f);

        // Izveido līnijas pārbaudi, lai mijiedarbotos ar kādu objektu
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(this);
        FHitResult TraceHit;

        // veic līnijas pārbaudi starp diviem punktiem, lai noteiktu, vai ir kāds objekts 
        // spēles pasaulē, ar kuru spēlētājs var mijiedarboties
        if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
        {
            // pārbauda, vai pārbaudītais objekts spēles pasaulē implementē UInteractionInterface interfeisu
            if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
            {
                //pārbauda vai pārbaudītais objekts ir mainījies salīdzinājumā ar iepriekšējo mijiedarbības objektu
                if (TraceHit.GetActor() != InteractionData.CurrentInteractable)
                {
                    //izsauc funkciju FoundInteractable(), padodot tai kā argumentu jauno mijiedarbības objektu
                    FoundInteractable(TraceHit.GetActor());
                }
            }
        }
        else
        {
            NoInteractableFound();
        }
    }
    else
    {
        NoInteractableFound();
    }
}


void ACilveks::FoundInteractable(AActor* NewInteractable)
{
    // Ja spēlētājs jau mijiedarbojas ar kādu objektu, tad pabeidz šo mijiedarbību
    if (IsInteracting())
    {
        EndInteract();
    }

    // Pārbauda, vai bija iepriekšējais mijiedarbības objekts
    if (InteractionData.CurrentInteractable)
    {
        TargetInteractable = InteractionData.CurrentInteractable;
        TargetInteractable->EndFocus(); // Ja bija, pabeidz tā fokusu
    }

    // Saglabā jauno mijiedarbības objektu kā pašreizējo
    InteractionData.CurrentInteractable = NewInteractable;
    TargetInteractable = NewInteractable;

    // Atjauno interakcijas logrīku ar jaunā objekta informāciju
    HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);

    // Uzsāk jaunā mijiedarbības objekta fokusu
    TargetInteractable->BeginFocus();
}

void ACilveks::NoInteractableFound() 
{
    // Ja bija sākta mijiedarbība ar kādu objektu, tad pabeidz to
    if (IsInteracting())
    {
        GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
    }

    // Ja bija iepriekšējais mijiedarbības objekts, tad pabeidz tam fokusu
    if (InteractionData.CurrentInteractable)
    {
        if (IsValid(TargetInteractable.GetObject()))
        {
            TargetInteractable->EndFocus();
        }

        // Paslēpj interakcijas logrīku
        HUD->HideInteractionWidget();

        // nosaka pašreizējo mijiedarbības objektu kā nullptr
        InteractionData.CurrentInteractable = nullptr;
        TargetInteractable = nullptr;
    }
}

void ACilveks::BeginInteract()
{
    // Pārbauda, vai kopš pēdējās mijiedarbības pārbaudes nav mainījies mijiedarbības objekts
    PerformInteractionCheck();

    //pārbauda, vai pašreizējais mijiedarbības objekts ir derīgs
    if (InteractionData.CurrentInteractable)
    {
        //pārbauda, vai TargetInteractable ir derīgs
        if (IsValid(TargetInteractable.GetObject()))
        {
            // Uzsāk mijiedarbību ar objektu
            TargetInteractable->BeginInteract();

            //pārbauda, vai mijiedarbības ilgums ar pašreizējo objektu ir gandrīz nulle
            if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
            {
                Interact();
            }
            else 
            {
                // Ja ne, uzstāda taimeri, lai pēc noteiktā laika pabeigtu interakciju
                GetWorldTimerManager().SetTimer(TimerHandle_Interaction,
                    this,
                    &ACilveks::Interact,
                    TargetInteractable->InteractableData.InteractionDuration,
                    false);
            }
        }
    }
}

void ACilveks::EndInteract() 
{
    // Notīra taimeri, kas tiek izmantots, lai pabeigtu mijiedarbību ar objektu
    GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

    // Pārbauda, vai pašreizējais mijiedarbības objekts ir derīgs
    if (IsValid(TargetInteractable.GetObject()))
    {
        // Ja tā ir, pabeidz mijiedarbību ar objektu, izsaucot tā EndInteract() funkciju
        TargetInteractable->EndInteract();
    }
}

void ACilveks::Interact() 
{
    // Notīra taimeri, kas ir izmantots, lai pabeigtu mijiedarbību ar objektu
    GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

    // Pārbauda, vai pašreizējais mijiedarbības objekts ir derīgs
    if (IsValid(TargetInteractable.GetObject()))
    {
        // Ja tā ir, veic mijiedarbību ar objektu, izsaucot tā Interact() funkciju
        TargetInteractable->Interact(this);
    }
}

void ACilveks::UpdateInteractionWidget() const
{
    // Pārbauda, vai pašreizējais mijiedarbības objekts ir derīgs
    if (IsValid(TargetInteractable.GetObject()))
    {
        // Ja tā ir, izsauc HUD klases UpdateInteractionWidget() funkciju,
        // lai atjaunotu mijiedarbības logrīka informāciju
        HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);
    }
}

void ACilveks::DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop)
{
    // Pārbauda, vai spēlētāja inventory satur nometamo objektu un, ja tas to satur, cik daudz
    if (PlayerInventory->FindMatchingItem(ItemToDrop))
    {
        // Iestata nometamā objekta rašanās parametrus
        FActorSpawnParameters SpawnParams;
        // spēlētājam piederēs objekts
        SpawnParams.Owner = this;
        // neizdot kļūdu, ja objekts nevar tikt izveidots
        SpawnParams.bNoFail = true;
        // jāizvairās no sadursmēm ar citiem objektiem, taču ja to nevar tikt izveidotam tāpat
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        // objektu izveido un nomet priekšā spēlētājam
        const FVector SpawnLocation{ GetActorLocation() + (GetActorForwardVector() * 50.0f) };

        // nosaka atrašanās vietu un lokāciju
        const FTransform SpawnTransform(GetActorRotation(), SpawnLocation);

        // nosaka izņemto daudzumu
        const int32 RemovedQuantity = PlayerInventory->RemoveAmountOfItem(ItemToDrop, QuantityToDrop);

        // izveido jaunu nometamo objektu
        APickup* Pickup = GetWorld()->SpawnActor<APickup>(APickup::StaticClass(), SpawnTransform, SpawnParams);

        // Inicializē nometamo objektu, nosakot nometamo objektu, daudzumu un citu nepieciešamo informāciju
        Pickup->InitializeDrop(ItemToDrop, QuantityToDrop, MyItemDataTable);
    }
}

void ACilveks::ToggleMenu()
{
    // Izsauc HUD klases ToggleMenu() funkciju, lai pārslēgtu izvēlni
    HUD->ToggleMenu();
}

float ACilveks::GetHealth() const
{
    // atgriež dzīvību vērtību
    return Health;
}

float ACilveks::GetMaxHealth() const
{
    // atgriež maksimālo dzīvību vērtību
    return MaxHealth;
}

void ACilveks::SetHealth(float const NewHealth)
{
    // Iestata jauno health vērtību
    Health = NewHealth;

    // Pārbauda, vai šis objekts ir spēlētājs
    if (Cast<ACilveks>(this))
    {
        // Ja veselība ir mazāka vai vienāda ar nulli, tad spēlētājs ir zaudējis
        if (Health <= 0)
        {
            UE_LOG(LogTemp, Error, TEXT("You Lose!"));
            GetWorld()->GetFirstPlayerController()->ConsoleCommand("Quit");
        }
    }
}

void ACilveks::ToggleTradingPanel()
{
    UE_LOG(LogTemp, Error, TEXT("Cilveks toggle trading panel activated"));

    // Pārbauda, vai ir esošs tirdzniecības centrs
    if (CurrentCenter)
    {
        UE_LOG(LogTemp, Warning, TEXT("J key pressed"));
        // Izsauc tirdzniecības paneļa pārslēgšanu esošajā centrā
        CurrentCenter->ToggleTradingPanel();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("CurrentCenter is nullptr!"));
    }
}

void ACilveks::ToggleStartScreenWidget()
{
    // pārbauda, vai spēles pasaule un autoritatīvais spēles režīms ir pieejami, pirms turpināt
    if (GetWorld() && GetWorld()->GetAuthGameMode())
    {
        // Iegūst spēles režīma instanci
        ASpeeleGameMode* GameMode = Cast<ASpeeleGameMode>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            // Izsauc spēles režīma funkciju, lai pārslēgtu sākuma ekrāna logrīku
            GameMode->ToggleStartScreenWidget();
        }
    }
}



