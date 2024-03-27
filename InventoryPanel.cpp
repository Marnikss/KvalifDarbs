// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/Inventory/InventoryPanel.h"
#include "Speele/Cilveks.h"
#include <UserInterface/Inventory/ItemDragDropOperation.h>

void UInventoryPanel::RefreshInventory()
{
	if (InventoryReference && InventorySlotClass)
	{
		InventoryPanel->ClearChildren();

		for (UItemBase* const& InventoryItem : InventoryReference->GetInventoryContents())
		{
			UInventoryItemSlot* ItemSlot = CreateWidget<UInventoryItemSlot>(this, InventorySlotClass);
			ItemSlot->SetItemReference(InventoryItem);

			InventoryPanel->AddChildToWrapBox(ItemSlot);
		}
		SetInfoText();
	}
}

void UInventoryPanel::SetInfoText() const
{
	//WeightInfo->SetText(FText::Format(FText::FromString("{0}/{1}"), 
				//InventoryReference->GetInventoryTotalWeight(),
				//InventoryReference->GetWeightCapacity()));
	CapacityInfo->SetText(FText::Format(FText::FromString("{0}/{1}"),
				InventoryReference->GetInventoryContents().Num(),
				InventoryReference->GetSlotsCapacity()));
}


void UInventoryPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerCharacter = Cast<ACilveks>(GetOwningPlayerPawn());
	if (PlayerCharacter)
	{
		InventoryReference = PlayerCharacter->GetInventory();
		if (InventoryReference)
		{
			InventoryReference->OnInventoryUpdated.AddUObject(this, &UInventoryPanel::RefreshInventory);
			SetInfoText();
		}
	}
}

bool UInventoryPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);

	if (ItemDragDrop->SourceItem && InventoryReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("Detected an item drop on InventoryPanel."));

		// returning true will stop the drop operation
		return true;
	}
	return false;
}