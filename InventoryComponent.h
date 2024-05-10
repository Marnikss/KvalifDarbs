#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Items/ItemBase.h>
#include "InventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdated);

UENUM(BlueprintType)
enum class EItemAddResult : uint8
{
	IAR_NoItemAdded UMETA(DisplayName = "No Item Added"),
	IAR_PartialAmountItemAdded UMETA(DisplayName = "Partial amount of Item Added"),
	IAR_AllItemAdded UMETA(DisplayName = "All Item Added")
};

USTRUCT(BlueprintType)
struct FItemAddResult
{
	GENERATED_BODY()

	FItemAddResult() :
		ActualAmountAdded(0),
		OperationResult(EItemAddResult::IAR_NoItemAdded),
		ResultMessage(FText::GetEmpty()) {};

	// Actual amount of item that was added to the inventory
	UPROPERTY(BlueprintReadOnly, Category="Item Add Result")
	int32 ActualAmountAdded;
	// Enum representing the end state of an add item operation
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	EItemAddResult OperationResult;
	// Informational message that can be passed with the result
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	FText ResultMessage;

	static FItemAddResult AddedNone(const FText& ErrorText)
	{
		FItemAddResult AddedNoneResult;
		AddedNoneResult.ActualAmountAdded = 0;
		AddedNoneResult.OperationResult = EItemAddResult::IAR_NoItemAdded;
		AddedNoneResult.ResultMessage = ErrorText;
		return AddedNoneResult;
	};
	static FItemAddResult AddedPartial(const int32 PartialAmountAdded, const FText& ErrorText)
	{
		FItemAddResult AddedPartialResult;
		AddedPartialResult.ActualAmountAdded = PartialAmountAdded;
		AddedPartialResult.OperationResult = EItemAddResult::IAR_PartialAmountItemAdded;
		AddedPartialResult.ResultMessage = ErrorText;
		return AddedPartialResult;
	};

	static FItemAddResult AddedAll(const int32 AmountAdded, const FText& Message)
	{
		FItemAddResult AddedAllResult;
		AddedAllResult.ActualAmountAdded = AmountAdded;
		AddedAllResult.OperationResult = EItemAddResult::IAR_AllItemAdded;
		AddedAllResult.ResultMessage = Message;
		return AddedAllResult;
	};
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPEELE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	//===================================================
	// PROPERTIES & VARIABLES
	//===================================================

	bool HasItemWithID(const int32 ItemID) const;

	

	UPROPERTY(EditInstanceOnly, Category = "Inventory")
	int32 InventorySlotsCapacity;

	FOnInventoryUpdated OnInventoryUpdated;

	UPROPERTY(EditInstanceOnly, Category = "Inventory")
	TArray<TObjectPtr<UItemBase>> InventoryContents;

	//===================================================
	// FUNCTIONS
	//===================================================

	//TArray<UItemBase*>& GetInventoryItems() { return InventoryItems; }

	void RemoveItemByID(int32 ItemID);

	void AddNewItem(UItemBase* Item, const int32 AmountToAdd);

	UInventoryComponent();

	UFUNCTION(Category = "Inventory")
	UItemBase* FindMatchingItem(UItemBase* ItemIn) const;

	UFUNCTION(Category = "Inventory")
	UItemBase* FindNextItemByID(UItemBase* ItemIn) const;

	UFUNCTION(Category = "Inventory")
	UItemBase* FindNextPartialStack(UItemBase* ItemIn) const;

	UFUNCTION(Category = "Inventory")
	FItemAddResult HandleAddItem(UItemBase* InputItem);

	UFUNCTION(Category = "Inventory")
	void RemoveSingleInstanceOfItem(UItemBase* ItemToRemove);

	UFUNCTION(Category = "Inventory")
	int32 RemoveAmountOfItem(UItemBase* ItemIn, int32 DesiredAmountToRemove);

	UFUNCTION(Category = "Inventory")
	void SplitExistingStack(UItemBase* ItemIn, const int32 AmountToSplit);

	// Getters
	UFUNCTION(Category = "Inventory")
	FORCEINLINE float GetInventoryTotalWeight() const { return InventoryTotalWeight; };
	UFUNCTION(Category = "Inventory")
	FORCEINLINE float GetWeightCapacity() const { return InventoryWeightCapacity; };
	UFUNCTION(Category = "Inventory")
	FORCEINLINE int32 GetSlotsCapacity() const { return InventorySlotsCapacity;  };
	UFUNCTION(Category = "Inventory")
	FORCEINLINE TArray<UItemBase*> GetInventoryContents() const { return InventoryContents; };

	FORCEINLINE void SetSlotsCapacity(const int32 NewSlotsCapacity);

	UFUNCTION(Category = "Inventory")
	FORCEINLINE void SetWeightCapacity(const float NewWeightCapacity) { InventoryWeightCapacity = NewWeightCapacity; };


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TArray<UItemBase*> Inventory;
protected:

	//===================================================
	// PROPERTIES & VARIABLES
	//===================================================

	TArray<UItemBase*> InventoryItems;

	UPROPERTY(VisibleAnywhere, Category="Inventory")
	float InventoryTotalWeight;
	
	UPROPERTY(EditInstanceOnly, Category = "Inventory")
	float InventoryWeightCapacity;

	//===================================================
	// FUNCTIONS
	//===================================================

	virtual void BeginPlay() override;

	FItemAddResult HandleNonStackableItems(UItemBase* InputItem);
	int32 HandleStackableItems(UItemBase* ItemIn, int32 RequestedAddAmount);
	int32 CalculateWeightAddAmount(UItemBase* ItemIn, int32 RequestedAddAmount);
	int32 CalculateNumberForFullStack(UItemBase* StackableItem, int32 InitialRequestedAddAmount);
		
	
	

	
};
