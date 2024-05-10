// ItemSpawner.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "World/Pickup.h"
#include "ItemSpawner.generated.h"

UCLASS()
class SPEELE_API AItemSpawner : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AItemSpawner();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Box component to define the area where items will spawn
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UBoxComponent* SpawnArea;

    // Class of the pickup item to spawn
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
    TSubclassOf<class APickup> PickupClass;

    // Number of items to spawn
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
    int32 NumberOfItems;

    // Range to randomize the spawn location within the box
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
    FVector2D RandomLocationRange;

    UPROPERTY(EditAnywhere, Category = "Data")
    UDataTable* MyItemDataTable; // This is the reference to your data table

private:
    void SpawnItemsRandomly();
};

