// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include <Components/BoxComponent.h>
#include "Pickup.generated.h"


class UDataTable;
class UItemBase;
class ACilveks;
class UInventoryComponent;

UCLASS()
class SPEELE_API APickup : public AActor, public IInteractionInterface 
{
	GENERATED_BODY()
	
public:	

	//===================================================
	// PROPERTIES & VARIABLES
	//===================================================
	bool bPickedUp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	bool bAxeOverlap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemData")
	TSubclassOf<class UItemBase> ItemBaseClass;




	UPROPERTY(VisibleAnywhere, Category = "Pickup | Components")
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	UDataTable* ItemDataTable;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	FName DesiredItemID;

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Item Reference")
	UItemBase* ItemReference;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	int32 ItemQuantity;

	UPROPERTY(VisibleInstanceOnly, Category = "Pickup | Interaction")
	FInteractableData InstanceInteractableData;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	int32 ItemID;


	//===================================================
	// FUNCTIONS
	//===================================================

	APickup();

	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void Pickup(ACilveks* PlayerCharacter);

	void DropThePickup(ACilveks* PlayerCharacter);

	void InitializeNpcDrop(UDataTable* InItemDataTable, const FName& InItemDataTableRowName, int32 InQuantity);

	void InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity);

	void InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity, UDataTable* InItemDataTable);

	UFUNCTION()
	void OnAxeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnAxeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void BladeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void BladeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FORCEINLINE UItemBase* GetItemData() { return ItemReference; };

	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	

protected:

	//===================================================
	// PROPERTIES & VARIABLES
	//===================================================


	


	


	//===================================================
	// FUNCTIONS
	//===================================================
	virtual void BeginPlay() override;

	virtual void Interact(ACilveks* PlayerCharacter) override;

	void UpdateInteractableData(); 

	void TakePickup(const ACilveks* Taker);

	void PrintItemProperties(UItemBase* Item);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

};
