// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
#include <Interfaces/InteractionInterface.h>





// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	// Log the number of items in the inventory after a delay of 1 second
	
}


FItemAddResult UInventoryComponent::HandleAddItem(UItemBase* InputItem)
{
	if (GetOwner())
	{
		const int32 InitialRequestedAddAmount = InputItem->Quantity;

		// Handle Non-stackable items
		if (!InputItem->NumericData.bIsStackable)//!
		{
			return HandleNonStackableItems(InputItem);
		}

		// Handle Stackable items
		const int32 StackableAmountAdded = HandleStackableItems(InputItem, InitialRequestedAddAmount);

		if (StackableAmountAdded == InitialRequestedAddAmount)
		{
			return FItemAddResult::AddedAll(InitialRequestedAddAmount, FText::Format(
				FText::FromString("Successfully added {0} {1} to the inventory."),
				InitialRequestedAddAmount,
				InputItem->TextData.Name));
		}

		if (StackableAmountAdded < InitialRequestedAddAmount && StackableAmountAdded > 0)
		{
			return FItemAddResult::AddedPartial(StackableAmountAdded, FText::Format(
				FText::FromString("Partial amount of {0} added to the inventory, Number added = {1}"),
				InputItem->TextData.Name,
				StackableAmountAdded));
		}

		if (StackableAmountAdded <= 0)
		{
			return FItemAddResult::AddedNone(FText::Format(
				FText::FromString("Couldnt add {0} to the inventory. No remaining inventory slots, or invalid item."),
				InputItem->TextData.Name));
		}
	}
	check(false);
	return FItemAddResult::AddedNone(FText::FromString("TryAddItem fallthrough error. GetOwner() check somehow failed."));
}

UItemBase* UInventoryComponent::FindMatchingItem(UItemBase* ItemIn) const
{
	if (ItemIn)
	{
		if (InventoryContents.Contains(ItemIn))
		{
			return ItemIn;
		}
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindNextItemByID(UItemBase* ItemIn) const
{
	if (ItemIn)
	{
		if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result = InventoryContents.FindByKey(ItemIn))
		{
			return *Result;
		}
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindNextPartialStack(UItemBase* ItemIn) const
{
	if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result =
		InventoryContents.FindByPredicate([&ItemIn](const UItemBase* InventoryItem)
			{
				return InventoryItem->ID == ItemIn->ID && !InventoryItem->IsFullItemStack();
			}
		))
	{
		return *Result;
	}
	return nullptr;
}

void UInventoryComponent::RemoveSingleInstanceOfItem(UItemBase* ItemToRemove)
{
	InventoryContents.RemoveSingle(ItemToRemove);
	OnInventoryUpdated.Broadcast();
}

int32 UInventoryComponent::RemoveAmountOfItem(UItemBase* ItemIn, int32 DesiredAmountToRemove)
{
	const int32 ActualAmountToRemove = FMath::Min(DesiredAmountToRemove, ItemIn->Quantity);

	ItemIn->SetQuantity(ItemIn->Quantity - ActualAmountToRemove);

	InventoryTotalWeight -= ActualAmountToRemove * ItemIn->GetItemSingleWeight();

	OnInventoryUpdated.Broadcast();

	return ActualAmountToRemove;
}

void UInventoryComponent::SplitExistingStack(UItemBase* ItemIn, const int32 AmountToSplit)
{
	if (!(InventoryContents.Num() + 1 > InventorySlotsCapacity))
	{
		RemoveAmountOfItem(ItemIn, AmountToSplit);
		AddNewItem(ItemIn, AmountToSplit);
	}
}

void UInventoryComponent::SetSlotsCapacity(const int32 NewSlotsCapacity)
{
	InventorySlotsCapacity = NewSlotsCapacity;
}

FItemAddResult UInventoryComponent::HandleNonStackableItems(UItemBase* InputItem)
{

	// check if in the input item has valid weight
	if (FMath::IsNearlyZero(InputItem->GetItemSingleWeight()) || InputItem->GetItemSingleWeight() < 0)
	{
		return FItemAddResult::AddedNone(FText::Format
		(FText::FromString("Could not add {0} to the inventory, item has invalid weight value."), InputItem->TextData.Name));
	}

	// will the item weight overflow weight capacity
	if (InventoryTotalWeight + InputItem->GetItemSingleWeight() > GetWeightCapacity())
	{
		return FItemAddResult::AddedNone(FText::Format
		(FText::FromString("Could not add {0} to the inventory, item would overflow weight limit."), InputItem->TextData.Name));
	}

	// adding one more item would overflow slot capacity
	if (InventoryContents.Num() + 1 > InventorySlotsCapacity)
	{
		return FItemAddResult::AddedNone(FText::Format
		(FText::FromString("Could not add {0} to the inventory, All inventory slots are full."), InputItem->TextData.Name));
	}
	
	AddNewItem(InputItem, 1); // 1
	UE_LOG(LogTemp, Warning, TEXT("Item added. Inventory size: %d"), InventoryContents.Num());
	return FItemAddResult::AddedAll(1, FText::Format //1
	(FText::FromString("Successfully added a single {0} to the inventory. HANDLE NONSTACKABLE ITEMS DONE"), InputItem->TextData.Name));
}


int32 UInventoryComponent::HandleStackableItems(UItemBase* ItemIn, int32 RequestedAddAmount)
{
	if (RequestedAddAmount <= 0 || FMath::IsNearlyZero(ItemIn->GetItemStackWeight()))
	{
		// invalid item data
		return 0;
	}

	int32 AmountToDistribute = RequestedAddAmount;

	// check if the input item already exists in the inventory and is not a full stack
	UItemBase* ExistingItemStack = FindNextPartialStack(ItemIn);

	//distribute item stack over existing stacks
	while (ExistingItemStack)
	{
		//calculate how many of the existing item would be needed to make the next full stack
		const int32 AmountToMakeFullStack = CalculateNumberForFullStack(ExistingItemStack, AmountToDistribute);
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ExistingItemStack, AmountToMakeFullStack);

		if (WeightLimitAddAmount > 0)
		{
			ExistingItemStack->SetQuantity(ExistingItemStack->Quantity + WeightLimitAddAmount);
			InventoryTotalWeight += (ExistingItemStack->GetItemSingleWeight() * WeightLimitAddAmount);

			AmountToDistribute -= WeightLimitAddAmount;

			ItemIn->SetQuantity(AmountToDistribute);

			if (InventoryTotalWeight >= InventoryWeightCapacity)
			{
				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount - AmountToDistribute;
			}
		}
		else if (WeightLimitAddAmount <= 0)
		{
			if (AmountToDistribute != RequestedAddAmount)
			{
				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount - AmountToDistribute;
			}
			return 0;
		}

		if (AmountToDistribute <= 0)
		{
			//all of the input item was distributed across existing stacks
			OnInventoryUpdated.Broadcast();
			return RequestedAddAmount;
		}

		//check if there is still another valid partial stack of the input item
		ExistingItemStack = FindNextPartialStack(ItemIn);
	}

	
	// no more partial stacks found, check if a new stack can be added
	if (InventoryContents.Num() + 1 <= InventorySlotsCapacity)
	{
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ItemIn, AmountToDistribute);

		if (WeightLimitAddAmount > 0)
		{
			if (WeightLimitAddAmount < AmountToDistribute)
			{
				AmountToDistribute -= WeightLimitAddAmount;
				ItemIn->SetQuantity(AmountToDistribute);

				//create a copy since only a partial stack is being addded
				AddNewItem(ItemIn->CreateItemCopy(), WeightLimitAddAmount);
				//ItemIn->SetQuantity(AmountToDistribute);//
				return RequestedAddAmount - AmountToDistribute;
			}
			//ItemIn->SetQuantity(RequestedAddAmount);//
			AddNewItem(ItemIn, AmountToDistribute);
			return RequestedAddAmount;
		}
		
	}

	OnInventoryUpdated.Broadcast();
	return RequestedAddAmount - AmountToDistribute;
}





int32 UInventoryComponent::CalculateWeightAddAmount(UItemBase* ItemIn, int32 RequestedAddAmount)
{
	const int32 WeightMaxAddAmount = FMath::FloorToInt((GetWeightCapacity() - InventoryTotalWeight) / ItemIn->GetItemSingleWeight());
	if (WeightMaxAddAmount >= RequestedAddAmount)
	{
		return RequestedAddAmount;
	}
	return WeightMaxAddAmount;
}

int32 UInventoryComponent::CalculateNumberForFullStack(UItemBase* StackableItem, int32 InitialRequestedAddAmount)
{
	const int32 AddAmountToMakeFullStack = StackableItem->NumericData.MaxStackSize - StackableItem->Quantity;

	return FMath::Min(InitialRequestedAddAmount, AddAmountToMakeFullStack);
}



void UInventoryComponent::AddNewItem(UItemBase* Item, const int32 AmountToAdd)
{
	UItemBase* NewItem;

	if (Item->bIsCopy || Item->bIsPickup)
	{
		// if the item is already a copy, or is a world pickup
		NewItem = Item;
		NewItem->ResetItemFlags();
	}
	else
	{
		// used when splitting or dragging to/from another inventory
		NewItem = Item->CreateItemCopy();
	}

	NewItem->OwningInventory = this;
	NewItem->SetQuantity(AmountToAdd);

	InventoryContents.Add(NewItem);
	InventoryTotalWeight += NewItem->GetItemStackWeight();
	UE_LOG(LogTemp, Warning, TEXT("Item added. Inventory size: %d"), InventoryContents.Num());
	UE_LOG(LogTemp, Warning, TEXT("Inventory Capacity: %d"), InventorySlotsCapacity); // Add this line to check inventory capacity
	OnInventoryUpdated.Broadcast();
}



bool UInventoryComponent::HasItemWithID(const int32 ItemID) const
{
	FName ItemIDName = FName(*FString::FromInt(ItemID));
	for (const auto& Item : InventoryContents)
	{
		if (Item && Item->ID.IsEqual(ItemIDName))
		{
			UE_LOG(LogTemp, Warning, TEXT("Item with ID %d found in inventory."), ItemID);
			return true;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Item with ID %d not found in inventory."), ItemID);
	return false;
}

void UInventoryComponent::RemoveItemByID(int32 ItemID)
{
	// Convert the integer ItemID to FString
	FString ItemIDString = FString::FromInt(ItemID);

	for (int32 Index = 0; Index < InventoryContents.Num(); ++Index)
	{
		if (InventoryContents[Index] && InventoryContents[Index]->ID.ToString() == ItemIDString)
		{
			// Remove the item from the inventory
			InventoryContents.RemoveAt(Index);
			UE_LOG(LogTemp, Warning, TEXT("Item with ID %d removed from inventory"), ItemID);
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Item with ID %d not found in inventory"), ItemID);
}

