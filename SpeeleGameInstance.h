// SpeeleGameInstance.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SpeeleSaveGame.h"
#include "SpeeleGameInstance.generated.h"


/**
 *
 */
UCLASS()
class SPEELE_API USpeeleGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    USpeeleGameInstance();

    // Save game data
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SaveGameData")
    USpeeleSaveGame* SaveGameData;

    // Function to save game
    UFUNCTION(BlueprintCallable)
    void SaveGame();

    // Function to load game
    UFUNCTION(BlueprintCallable)
    void LoadGame();

    UFUNCTION(BlueprintCallable)
    void SetPlayerPosition(const FVector& NewPosition);

    UFUNCTION(BlueprintCallable)
    void SetInventory(const TArray<FInventoryItem>& NewInventory);
};

