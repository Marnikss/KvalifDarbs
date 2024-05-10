// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "PatrolPath.h"
#include "CombatInterface.h"
#include "Animation/AnimMontage.h"
#include "CilveksBase.h"
#include "World/Pickup.h"
#include "NPC.generated.h"

class APickup;
class UItemBase;


UCLASS()
class SPEELE_API ANPC : public ACilveksBase, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UBehaviorTree* GetBehaviorTree() const;

	APatrolPath* GetPatrolPath() const;

	UAnimMontage* GetMontage() const;

	int MeleeAttack_Implementation() override;

	void DeathSequence();

	UAnimMontage* GetDeathMontage() const;

	float GetHealth() const;

	float GetMaxHealth() const;

	void SetHealth(float const NewHealth);

	void LootSpawn(UItemBase* ItemBase);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UBlueprint* BP_PickupBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess="true"))
	UBehaviorTree* Tree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	APatrolPath* PatrolPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Montage;
};
