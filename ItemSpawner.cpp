// ItemSpawner.cpp

#include "Items/ItemSpawner.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include <Speele/ItemDataStructs.h>
#include <Speele/Cilveks.h>

// Sets default values
AItemSpawner::AItemSpawner()
{
    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    // Create and attach box component
    SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
    RootComponent = SpawnArea;

    // Default values
    NumberOfItems = 10;
    RandomLocationRange = FVector2D(-500, 500);
}

// Called when the game starts or when spawned
void AItemSpawner::BeginPlay()
{
    Super::BeginPlay();

    if (MyItemDataTable == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Item data table not set in Item Spawner"));
    }
    else
    {
        SpawnItemsRandomly();
    }
}

// Called every frame
void AItemSpawner::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}










void AItemSpawner::SpawnItemsRandomly()
{
    if (PickupClass == nullptr || NumberOfItems <= 0 || MyItemDataTable == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid PickupClass, NumberOfItems, or ItemDataTable"));
        return;
    }

    // Get the bounds of the spawn area
    FVector MinBounds = SpawnArea->GetComponentLocation() - SpawnArea->GetScaledBoxExtent();
    FVector MaxBounds = SpawnArea->GetComponentLocation() + SpawnArea->GetScaledBoxExtent();

    // Loop to spawn items randomly
    for (int32 i = 0; i < NumberOfItems; ++i)
    {
        // Calculate a random spawn location within the box area
        FVector SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(MinBounds, MaxBounds);

        // Apply random location range
        SpawnLocation.X += FMath::FRandRange(RandomLocationRange.X, RandomLocationRange.Y);
        SpawnLocation.Y += FMath::FRandRange(RandomLocationRange.X, RandomLocationRange.Y);

        // Spawn the pickup item at the random location
        APickup* SpawnedPickup = GetWorld()->SpawnActor<APickup>(PickupClass, SpawnLocation, FRotator::ZeroRotator);

        if (SpawnedPickup)
        {
            float RandomProbability = FMath::FRand();
            if (RandomProbability <= 0.2f)
            {
                // 20% chance to spawn item with ID 6
                SpawnedPickup->InitializeNpcDrop(MyItemDataTable, FName("6"), 1);
            }
            else
            {
                // 80% chance to spawn item with ID 5
                SpawnedPickup->InitializeNpcDrop(MyItemDataTable, FName("5"), 1);
            }

            //UE_LOG(LogTemp, Warning, TEXT("Spawned Pickup: %s at location: %s with ID: %s and quantity: %d"), *SpawnedPickup->GetName(), *SpawnLocation.ToString(), *SpawnedPickup->DesiredItemID.ToString(), SpawnedPickup->ItemQuantity);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to spawn Pickup."));
        }
    }
}