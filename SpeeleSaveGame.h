#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SpeeleSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FInventoryItem
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemName;
};

UCLASS()
class SPEELE_API USpeeleSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    USpeeleSaveGame();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FVector PlayerPosition;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TArray<FInventoryItem> Inventory;
};
