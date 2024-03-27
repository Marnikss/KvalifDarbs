// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/WrapBox.h>
#include <Components/TextBlock.h>
#include "InventoryItemSlot.h"
#include "InventoryPanel.generated.h"



class UInventoryComponent;
class ACilveks;
/**
 * 
 */
UCLASS()
class SPEELE_API UInventoryPanel : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void RefreshInventory();

	UPROPERTY(meta=(BindWidget))// pasaka, ka ja sis elements netiks ieklauts blueprint widgetaa, 
		//kods neies
	UWrapBox* InventoryPanel;

	UPROPERTY()
	UTextBlock* WeightInfo;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* CapacityInfo;

	UPROPERTY()
	ACilveks* PlayerCharacter;
	UPROPERTY()
	UInventoryComponent* InventoryReference;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryItemSlot> InventorySlotClass;

protected:
	void SetInfoText() const;
	virtual void NativeOnInitialized() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;



};
