// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemBase.h"
#include "Components/InventoryComponent.h"
#include "Engine/DataTable.h"
#include <World/Pickup.h>

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
			//UE_LOG(LogTemp, Error, TEXT("ItemBase OwningInventory was null"));
		}
	}
}

void UItemBase::Use(ACharacter* CHaracter)
{

}

bool UItemBase::IsAxe() const
{
	UE_LOG(LogTemp, Warning, TEXT("Item ID: %s"), *ID.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Item Type: %d"), static_cast<int32>(ItemType));

	return ItemType == EItemType::Weapon;
}


float UItemBase::GetDamage() const
{
	if (IsAxe() && ItemStatistics.DamageValue > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("DamageValue: %f"), ItemStatistics.DamageValue);
		return ItemStatistics.DamageValue;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("DamageValue not initialized correctly or is not greater than 0"));
	}

	// Default value if damage value is not found or item is not an axe
	return 0.0f;
}


void UItemBase::InitializeFromDataTable(FName DataTableRowName, UDataTable* DataTable)
{
	if (!DataTable) return;

	FItemData* ItemData = DataTable->FindRow<FItemData>(DataTableRowName, "");
	if (ItemData)
	{
		ItemStatistics = ItemData->ItemStatistics;
	}
}








