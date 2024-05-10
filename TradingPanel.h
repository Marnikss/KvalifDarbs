// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Widget.h"
#include "Components/Button.h"
#include <World/Pickup.h>
#include "TradingPanel.generated.h"

class APickup;
/**
 * 
 */
UCLASS()
class SPEELE_API UTradingPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	UWidget* PanelWidget; // Widget you want to control visibility for

	UFUNCTION(BlueprintCallable)
	void ShowPanel();

	UFUNCTION(BlueprintCallable)
	void HidePanel();

	UFUNCTION(BlueprintCallable, Category = "Trading Panel")
	void TogglePanelVisibility(); // Add this function declaration

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	bool bPanelVisible = false;

	UFUNCTION()
	void OnButton1Clicked();

	UFUNCTION()
	void OnButton2Clicked();

	UFUNCTION()
	void OnButton3Clicked();

	UFUNCTION()
	void OnButton4Clicked();



	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* Button1;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* Button2;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* Button3;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* Button4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACilveks* PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInventoryComponent* PlayerInventory; // Reference to the player's inventory component
};
