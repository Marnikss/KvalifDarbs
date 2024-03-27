// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/Inventory/InventoryTooltip.h"
#include "UserInterface/Inventory/InventoryItemSlot.h"
#include "Items/ItemBase.h"
#include "Components/TextBlock.h"




void UInventoryTooltip::NativeConstruct()
{
	Super::NativeConstruct();

	const UItemBase* ItemBeingHovered = InventorySlotBeingHovered->GetItemReference();

	switch (ItemBeingHovered->ItemType)
	{
	case EItemType::Weapon:
		ItemType->SetText(FText::FromString("Tool"));
		break;
	default:;
	}

	ItemName->SetText(ItemBeingHovered->TextData.Name);
	DamageValue->SetText(FText::AsNumber(ItemBeingHovered->ItemStatistics.DamageValue));
	UsageText->SetText(ItemBeingHovered->TextData.UsageText);
	ItemDescription->SetText(ItemBeingHovered->TextData.Description);

	const FString WeightInfo =
	{ "Weight: " + FString::SanitizeFloat(ItemBeingHovered->GetItemStackWeight()) };

	//StackWeight->SetText(FText::FromString(WeightInfo));

	if (ItemBeingHovered->NumericData.bIsStackable)
	{
		const FString StackInfo =
		{ "Max stack size: " + FString::FromInt(ItemBeingHovered->NumericData.MaxStackSize) };

		//MaxStackSize->SetText(FText::FromString(StackInfo));
	}
	else
	{
		//MaxStackSize->SetVisibility(ESlateVisibility::Collapsed);
	}

	//StackWeightValue->SetText(FText::AsNumber(ItemBeingHovered->GetItemStackWeight()));

	if (ItemBeingHovered->NumericData.bIsStackable)
	{
		MaxStackSize->SetText(FText::AsNumber(ItemBeingHovered->NumericData.MaxStackSize));
	}
	else {
		MaxStackSize->SetVisibility(ESlateVisibility::Collapsed);
	}
}