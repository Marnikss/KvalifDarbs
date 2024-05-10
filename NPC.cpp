// Fill out your copyright notice in the Description page of Project Settings.
#include "AI_NPC/NPC.h"

#include "Speele/Cilveks.h"
#include "World/Pickup.h"


// Sets default values
ANPC::ANPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UBlueprint> PickupBaseFinder(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_PickupBase.BP_PickupBase'"));

	UE_LOG(LogTemp, Warning, TEXT("Finding BP_PickupBase!"));
	if (PickupBaseFinder.Succeeded())
	{
		BP_PickupBase = PickupBaseFinder.Object;
		UE_LOG(LogTemp, Warning, TEXT("BP_PickupBase asset found!"));
		UClass* PickupBaseClass = BP_PickupBase->GeneratedClass;
		if (PickupBaseClass != nullptr)
		{
			// Now you can use PickupBaseClass as the class for spawning your actor
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to get generated class from BP_PickupBase"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find BP_PickupBase asset"));
	}

}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UBehaviorTree* ANPC::GetBehaviorTree() const
{
	return Tree;
}

APatrolPath* ANPC::GetPatrolPath() const
{
	return PatrolPath;
}

UAnimMontage* ANPC::GetMontage() const
{
	return Montage;
}

int ANPC::MeleeAttack_Implementation()
{
	//UE_LOG(LogTemp, Warning, TEXT("Melee attack animation triggered"));

	if (Montage)
	{
		PlayAnimMontage(Montage);
	}
	return 0;
}



float ANPC::GetHealth() const
{
	return Health;
}

float ANPC::GetMaxHealth() const
{
	return MaxHealth;
}

void ANPC::SetHealth(float const NewHealth)
{
	Health = NewHealth;
	
	if (Cast<ANPC>(this))
	{
		if (Health <= 0)
		{
			Destroy();
			UE_LOG(LogTemp, Error, TEXT("You Win!"));
			//GetWorld()->GetFirstPlayerController()->ConsoleCommand("Quit");
			//DeathSequence();

			

			// Spawn the pickup item if the blueprint reference is valid
			//if (BP_PickupBase)
			//{
				// Spawn the pickup item
			//AActor* PickupItem = GetWorld()->SpawnActor<AActor>(BP_PickupBase->GeneratedClass, SpawnLocation, SpawnRotation);
			UItemBase* MyItemBase = NewObject<UItemBase>();
			// Initialize MyItemBase properties as needed
			LootSpawn(MyItemBase);
		}
		
	}
}

void ANPC::LootSpawn(UItemBase* ItemBase)
{
	// Spawn the pickup item if the blueprint reference is valid
	if (BP_PickupBase)
	{
		// Get the spawn location and rotation
		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();

		// Spawn the pickup item
		//AActor* PickupItem = GetWorld()->SpawnActor<AActor>(BP_PickupBase->GeneratedClass, SpawnLocation, SpawnRotation);
		APickup* PickupItem = GetWorld()->SpawnActor<APickup>(BP_PickupBase->GeneratedClass, SpawnLocation, SpawnRotation);

		// Check if the pickup item was spawned successfully
		if (PickupItem)
		{
			// Get reference to the data table by its name
			UDataTable* MyItemDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/ItemData/WeaponData.WeaponData'"));

			PickupItem->InitializeNpcDrop(MyItemDataTable,FName("4"), 1);
			// Assuming MyItemDataTable is the variable holding the reference to the data table
			if (MyItemDataTable)
			{
				UE_LOG(LogTemp, Warning, TEXT("Data table '%s' successfully retrieved."), *MyItemDataTable->GetName());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to retrieve data table."));
			}


			//PickupItem->InitializePickup(ItemDataTableName, "4", 1);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn pickup item"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BP_PickupBase reference is null"));
	}

	// Perform any additional actions related to loot spawning here

	// Destroy the NPC
	Destroy();
	UE_LOG(LogTemp, Error, TEXT("NPC Destroyed"));
}








