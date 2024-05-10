// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "CilveksBase.generated.h"


UCLASS()
class SPEELE_API ACilveksBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACilveksBase();

	class ANPC* CurrentlyOverlappingEnemy;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	float GetHealth() const;
	float GetMaxHealth() const;
	void SetHealth(float const NewHealth);
	void AttackStart();
	void AttackEnd();

	class UWidgetComponent* WidgetComponent;
	float const MaxHealth{ 100.f };
	float Health;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	class ACilveks* CurrentlyOverlappingPlayer;

	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Collision, meta=(AllowPrivateAccess="true"))
	class UBoxComponent* RightFistCollisionBox;

	UFUNCTION()
	void OnAttackOverlapBegin(
		UPrimitiveComponent* const OverlappedComponent,
		AActor* const OtherActor,
		UPrimitiveComponent* const OtherComponent,
		int const OtherBodyIndex,
		bool const FromSweep,
		FHitResult const& SweepResult);

	UFUNCTION()
	void OnAttackOverlapEnd(
		UPrimitiveComponent* const OverlappedComponent,
		AActor* const OtherActor,
		UPrimitiveComponent* OtherComponent,
		int const OtherBodyIndex);

	//UFUNCTION()
	//void PlayerAttackOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//UFUNCTION()
	//void PlayerAttackOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
