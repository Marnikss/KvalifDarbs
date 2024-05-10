// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Widget.h"
#include "Components/Button.h"

#include "StartScreenWidget.generated.h"

/**
 *
 */
UCLASS()
class SPEELE_API UStartScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void NativeConstruct();

	UFUNCTION()
	void OnSaveGameClicked();

	UFUNCTION()
	void OnLoadGameClicked();

	UFUNCTION()
	void OnNewGameClicked();

	UFUNCTION()
	void OnQuitGameClicked();



	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* LoadGameButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* NewGameButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* SaveGameButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* QuitGameButton;

	UPROPERTY()
	class ASpeeleGameMode* SpeeleGameMode; // Reference to game mode

};

