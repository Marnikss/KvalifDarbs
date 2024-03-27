// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CilvekaHUD.generated.h"

struct FInteractableData;
class UMainMenu;
class UInteractionWidget;

UCLASS()
class SPEELE_API ACilvekaHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	//===================================================
	// PROPERTIES & VARIABLES
	//===================================================

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UMainMenu> MainMenuClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;

	bool bIsMenuVisible;

	//===================================================
	// FUNCTIONS
	//===================================================

	ACilvekaHUD();

	void DisplayMenu();
	void HideMenu();
	void ToggleMenu();

	void ShowInteractionWidget() const;
	void HideInteractionWidget() const;
	void UpdateInteractionWidget(const FInteractableData* InteractableData) const;

protected:

	//===================================================
	// PROPERTIES & VARIABLES
	//===================================================

	UPROPERTY()
	UMainMenu* MainMenuWidget;

	UPROPERTY()
	UInteractionWidget* InteractionWidget;

	//===================================================
	// FUNCTIONS
	//===================================================

	virtual void BeginPlay() override;
};
