// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemBase.h"
#include "Components/InventoryComponent.h"

UItemBase::UItemBase() : bIsCopy(false), bIsPickup(false)
{

}

void UItemBase::ResetItemFlags()
{
	bIsCopy = false;
	bIsPickup = false;
}

UItemBase* UItemBase::CreateItemCopy()
{
	UItemBase* ItemCopy = NewObject<UItemBase>(StaticClass());

	ItemCopy->ID = this->ID;
	ItemCopy->Quantity = this->Quantity;
	ItemCopy->ItemQuality = this->ItemQuality;
	ItemCopy->ItemType = this->ItemType;
	ItemCopy->TextData = this->TextData;
	ItemCopy->NumericData = this->NumericData;
	ItemCopy->ItemStatistics = this->ItemStatistics;
	ItemCopy->AssetData = this->AssetData;
	ItemCopy->bIsCopy = true;

	return ItemCopy;
}

void UItemBase::SetQuantity(const int32 NewQuantity)
{
	if (NewQuantity != this->Quantity)
	{
		// Ja items ir Stackable, maximum value var but tikai max stack size, ja nav stackable, max value ir 1
		Quantity = FMath::Clamp(NewQuantity, 0, this->NumericData.bIsStackable ? this->NumericData.MaxStackSize : 1);

		if (OwningInventory)
		{
			 if (this->Quantity <= 0)
			 {
				this->OwningInventory->RemoveSingleInstanceOfItem(this);
			 }
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ItemBase OwningInventory was null"));
		}
	}
}

void UItemBase::Use(ACharacter* CHaracter)
{

}