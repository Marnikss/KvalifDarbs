// ACilveks.cpp

//game
#include "Cilveks.h"
#include "Speele.h"
#include "UserInterface/CilvekaHUD.h"
#include "Components/InventoryComponent.h"
#include "World/Pickup.h"


//engine
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"


ACilveks::ACilveks()
{
    PrimaryActorTick.bCanEverTick = true;
    

    GetCharacterMovement()->JumpZVelocity = 800.0f;
    GetCharacterMovement()->MaxWalkSpeed = 300.0f; // Set your desired maximum walk speed


    InteractionCheckFrequency = 0.1;
    InteractionCheckDistance = 225.0f;

    PlayerInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("PlayerINventory"));
    PlayerInventory->SetSlotsCapacity(20);
    PlayerInventory->SetWeightCapacity(50.0f);
}

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

    // Bind Sprint
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ACilveks::StartSprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ACilveks::StopSprint);

    PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACilveks::BeginInteract);
    PlayerInputComponent->BindAction("Interact", IE_Released, this, &ACilveks::EndInteract);


}

void ACilveks::WalkBackward(float Value)
{
    
	
    // Value will be positive when moving forward and negative when moving backward
    const float AdjustedValue = -Value;
    if (Controller != nullptr && AdjustedValue != 0.0f)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, AdjustedValue);
    }
}



void ACilveks::ToggleCrouch(float Value)
{
     
	
    if (Value > 0.0f && !GetCharacterMovement()->IsFalling())
    {
        // Positive value means the crouch action is pressed and not mid-air
        Crouch();
    }
    else
    {
        // Non-positive value or mid-air means the crouch action is released or disabled
        UnCrouch();
    }
}

void ACilveks::MoveForward(float Value)
{
    GetCharacterMovement()->bOrientRotationToMovement = (Value >= 0.0f);

    if (Controller != nullptr && Value != 0.0f)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}





void ACilveks::MoveRight(float Value)
{
     
    if (Controller != nullptr && Value != 0.0f)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(Direction, Value);
    }
}

void ACilveks::Turn(float Value)
{
    AddControllerYawInput(Value);
}



void ACilveks::LookUp(float Value)
{
   

    if (Controller != nullptr && Value != 0.0f)
    {
        AddControllerPitchInput(Value);
    }
}


void ACilveks::StartJump()
{
    bPressedJump = true;
}

void ACilveks::StopJump()
{
    bPressedJump = false;
}

void ACilveks::StartSprint()
{
    bIsSprinting = true;
    GetCharacterMovement()->MaxWalkSpeed *= 3.0f; // Increase speed when sprinting.
}

void ACilveks::StopSprint()
{
    bIsSprinting = false;
    GetCharacterMovement()->MaxWalkSpeed /= 3.0f; // Reset speed when not sprinting.
}

void ACilveks::BeginPlay()
{
    Super::BeginPlay();

    HUD = Cast<ACilvekaHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

    APlayerCameraManager* const camMan = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
    camMan->ViewPitchMin = -70.f;
    camMan->ViewPitchMax = 20.f;

}

void ACilveks::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency)
    {
        PerformInteractionCheck();
    }
}

void ACilveks::PerformInteractionCheck()
{
    InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

    FVector TraceStart{ GetPawnViewLocation() };
    FVector TraceEnd{ TraceStart + (GetViewRotation().Vector() * InteractionCheckDistance) };

    float LookDirection = FVector::DotProduct(GetActorForwardVector(), GetViewRotation().Vector());

    if (LookDirection > 0)
    {
        //DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f);

        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(this);
        FHitResult TraceHit;

        if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
        {
            if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
            {
                if (TraceHit.GetActor() != InteractionData.CurrentInteractable)
                {
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
    if (IsInteracting())
    {
        EndInteract();
    }

    if (InteractionData.CurrentInteractable)
    {
        TargetInteractable = InteractionData.CurrentInteractable;
        TargetInteractable->EndFocus();
    }

    InteractionData.CurrentInteractable = NewInteractable;
    TargetInteractable = NewInteractable;

    HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);

    TargetInteractable->BeginFocus();
}

void ACilveks::NoInteractableFound() 
{
    if (IsInteracting())
    {
        GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
    }

    if (InteractionData.CurrentInteractable)
    {
        if (IsValid(TargetInteractable.GetObject()))
        {
            TargetInteractable->EndFocus();
        }

        HUD->HideInteractionWidget();

        InteractionData.CurrentInteractable = nullptr;
        TargetInteractable = nullptr;
    }
}

void ACilveks::BeginInteract()
{
    //Verify nothing has changed with the interactable statesince beginning interaction
    PerformInteractionCheck();

    if (InteractionData.CurrentInteractable)
    {
        if (IsValid(TargetInteractable.GetObject()))
        {
            TargetInteractable->BeginInteract();

            if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
            {
                Interact();
            }
            else 
            {
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
    GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

    if (IsValid(TargetInteractable.GetObject()))
    {
        TargetInteractable->EndInteract();
    }
}

void ACilveks::Interact() 
{
    GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

    if (IsValid(TargetInteractable.GetObject()))
    {
        TargetInteractable->Interact(this);
    }
}

void ACilveks::UpdateInteractionWidget() const
{
    if (IsValid(TargetInteractable.GetObject()))
    {
        HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);
    }
}

void ACilveks::DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop)
{
    if (PlayerInventory->FindMatchingItem(ItemToDrop))
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.bNoFail = true;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        const FVector SpawnLocation{ GetActorLocation() + (GetActorForwardVector() * 50.0f) };

        const FTransform SpawnTransform(GetActorRotation(), SpawnLocation);

        const int32 RemovedQuantity = PlayerInventory->RemoveAmountOfItem(ItemToDrop, QuantityToDrop);

        APickup* Pickup = GetWorld()->SpawnActor<APickup>(APickup::StaticClass(), SpawnTransform, SpawnParams);

        Pickup->InitializeDrop(ItemToDrop, RemovedQuantity);
    }
}

void ACilveks::ToggleMenu()
{
    HUD->ToggleMenu();
}