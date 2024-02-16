#pragma once
#include "GameFramework/CharacterMovementComponent.h"
#include <Interfaces/InteractionInterface.h>
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Cilveks.generated.h"

class ACilvekaHUD;

USTRUCT()
struct FInteractionData
{
    GENERATED_BODY()

    FInteractionData() : CurrentInteractable(nullptr), LastInteractionCheckTime(0.0f)
    {

    };

    UPROPERTY()
    AActor* CurrentInteractable;

    UPROPERTY()
    float LastInteractionCheckTime;
};

UCLASS()
class SPEELE_API ACilveks : public ACharacter
{
    GENERATED_BODY()

 public:
    ACilveks();

    FORCEINLINE bool IsInteracting() const {
        return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction);
    }
    

 protected:
    //===================================================
    // PROPERTIES & VARIABLES
    //===================================================

     UPROPERTY()
     ACilvekaHUD* HUD;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    
    // virtual void Tick(float DeltaTime) override;

    
    UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
    TScriptInterface<IInteractionInterface> TargetInteractable;

    float InteractionCheckFrequency;

    float InteractionCheckDistance;

    FTimerHandle TimerHandle_Interaction;

    FInteractionData InteractionData;


 private:

    bool bIsSprinting;

    UPROPERTY(VisibleAnywhere)
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere)
    UCameraComponent* CameraComponent;
    
    //===================================================
    // FUNCTIONS
    //===================================================

    void PerformInteractionCheck();
    void FoundInteractable(AActor* NewInteractable);
    void NoInteractableFound();
    void BeginInteract();
    void EndInteract();
    void Interact();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

    void MoveForward(float Value);
    void MoveRight(float Value);
    void Turn(float Value);
    void LookUp(float Value);
    void StartJump();
    void StopJump();
    void WalkBackward(float Value);
    void ToggleCrouch(float Value);

    // void UpdateMovementSettings(); 
    

    // New function for sprinting
    void StartSprint();
    void StopSprint();

 
    
    
    
};
