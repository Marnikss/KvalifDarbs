// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Pickup.h"
#include "Speele/Cilveks.h"
#include "Components/InventoryComponent.h"
#include "Components/BoxComponent.h"
#include "Items/ItemBase.h"
#include <AI_NPC/NPC.h>
#include <Kismet/GameplayStatics.h>

class ACilveks;
class ACilveksBase;


APickup::APickup()
{
	
	PrimaryActorTick.bCanEverTick = true;//false

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	PickupMesh->SetSimulatePhysics(true);
	SetRootComponent(PickupMesh);

	


}

void APickup::BeginPlay()
{
	Super::BeginPlay();

	

	//PickupMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	InitializePickup(UItemBase::StaticClass(), ItemQuantity);
	

	UBoxComponent* AxeBox = FindComponentByClass<UBoxComponent>();

	bAxeOverlap = false;
	
	if (AxeBox)
	{
		AxeBox->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnAxeOverlapBegin);
		AxeBox->OnComponentEndOverlap.AddDynamic(this, &APickup::OnAxeOverlapEnd);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AxeBox is null!"));
	}

	UBoxComponent* AxeBladeColBox = FindComponentByClass<UBoxComponent>();

	if (AxeBladeColBox) {
		AxeBladeColBox->OnComponentBeginOverlap.AddDynamic(this, &APickup::BladeOverlapBegin);
		AxeBladeColBox->OnComponentEndOverlap.AddDynamic(this, &APickup::BladeOverlapEnd);
		//UE_LOG(LogTemp, Warning, TEXT("AxeBladeColBox exists"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AxeBladeColBox is null!"));
	}
	
}




void APickup::Pickup(ACilveks* PlayerCharacter)
{
	// Check if the player character is valid
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("Player character is not valid."));
	}

	// Check if the pickup mesh is valid
	if (!PickupMesh)
	{
		UE_LOG(LogTemp, Error, TEXT("Pickup mesh is not valid."));
	}

	if (bPickedUp == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("Axe already picked up, dropping initiated"));
		DropThePickup(PlayerCharacter);
		return;
	}

	// Add debugging statements to trace the flow
	UE_LOG(LogTemp, Warning, TEXT("Attempting to pick up the pickup."));
	UE_LOG(LogTemp, Warning, TEXT("bAxeOverlap: %d"), bAxeOverlap);

	USkeletalMeshComponent* PlayerMesh = PlayerCharacter->GetMesh();
	if (PlayerMesh)
	{
		FName AttachedSocket = PickupMesh->GetAttachSocketName();
		if (PlayerMesh->DoesSocketExist(AttachedSocket))
		{
			UE_LOG(LogTemp, Warning, TEXT("Something is already attached to AxeSocket."));
			DropThePickup(PlayerCharacter);
			return;
		}
	}

	if (bAxeOverlap)
	{
		
		// Attach the pickup mesh to the character's hand socket
		PickupMesh->AttachToComponent(PlayerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "AxeSocket");

		PickupMesh->SetSimulatePhysics(false);
		
		PlayerCharacter->bAxeEquipped = true;

		UE_LOG(LogTemp, Warning, TEXT("The axe is in range. Pickup function called."));

		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("What are you trying to pick up?!?!?."));
	}

	
	

}

void APickup::DropThePickup(ACilveks* PlayerCharacter)
{
	// Check if the player character is valid
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("Player character is not valid."));
		
	}

	UE_LOG(LogTemp, Error, TEXT("DROPTHEPICKUP"));
	// Detach the pickup from the character's hand
	PickupMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	PickupMesh->SetSimulatePhysics(true);
	
	

	bPickedUp = false;

	
}


void APickup::InitializeNpcDrop(UDataTable* InItemDataTable, const FName& InItemDataTableRowName, int32 InQuantity)
{
	// Set the data table, item quantity, and desired item ID
	ItemDataTable = InItemDataTable;
	ItemQuantity = InQuantity;
	DesiredItemID = InItemDataTableRowName;

	// Check if the data table is valid
	if (ItemDataTable)
	{
		// Retrieve row data from the data table
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());
		if (ItemData)
		{
			// Create a new item reference
			ItemReference = NewObject<UItemBase>(this);

			// Set the item properties from the row data
			ItemReference->ID = ItemData->ID;
			ItemReference->ItemType = ItemData->ItemType;
			ItemReference->ItemQuality = ItemData->ItemQuality;
			ItemReference->NumericData = ItemData->NumericData;
			ItemReference->TextData = ItemData->TextData;
			ItemReference->AssetData = ItemData->AssetData;

			// Set the item quantity
			InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);

			// Set the mesh of the PickupMesh component
			if (PickupMesh)
			{
				PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);
			}

			// Perform any additional setup or logic here
			UpdateInteractableData();
		}
		else
		{
			// Handle the case where ItemData is null
			UE_LOG(LogTemp, Warning, TEXT("Failed to find ItemData for DesiredItemID: %s"), *DesiredItemID.ToString());
		}
	}
	else
	{
		// Handle the case where ItemDataTable is null
		UE_LOG(LogTemp, Error, TEXT("Item data table is invalid."));
	}
}



void APickup::InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity)
{
	if (ItemDataTable && !DesiredItemID.IsNone())// !
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());
		if (ItemData)
		{
			// Check if the ItemReference is already initialized
			if (!ItemReference)
			{
				// Create a new item reference only if it's not already initialized
				ItemReference = NewObject<UItemBase>(this, BaseClass);
			}

			ItemReference->ID = ItemData->ID;
			ItemReference->ItemType = ItemData->ItemType;
			ItemReference->ItemQuality = ItemData->ItemQuality;
			ItemReference->NumericData = ItemData->NumericData;
			ItemReference->TextData = ItemData->TextData;
			ItemReference->AssetData = ItemData->AssetData;

			InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);

			ItemReference->InitializeFromDataTable(DesiredItemID, ItemDataTable);

			PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);

			UpdateInteractableData();
		}
		else
		{
			// Handle the case where ItemData is null
			UE_LOG(LogTemp, Warning, TEXT("Failed to find ItemData for DesiredItemID: %s"), *DesiredItemID.ToString());
		}
	}
}

void APickup::InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity, UDataTable* InItemDataTable)
{
	if (!ItemToDrop)
	{
		UE_LOG(LogTemp, Error, TEXT("ItemToDrop is nullptr in InitializeDrop"));
		return;
	}

	// Get the class of the BP_PickupBase blueprint
	UClass* BP_PickupBaseClass = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Game/Blueprints/BP_PickupBase.BP_PickupBase_C"));

	if (!BP_PickupBaseClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load BP_PickupBase blueprint class"));
		return;
	}

	// Spawn a new instance of the BP_PickupBase blueprint
	APickup* DroppedItem = GetWorld()->SpawnActor<APickup>(BP_PickupBaseClass, GetActorTransform());

	if (!DroppedItem)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn dropped item"));
		return;
	}
	
	//ItemDataTable = InItemDataTable; // Assign the data table reference

	DroppedItem->DesiredItemID = ItemToDrop->ID;
	DroppedItem->ItemQuantity = InQuantity;
	DroppedItem->ItemDataTable = InItemDataTable;

	// Initialize the pickup with the copied properties
	DroppedItem->InitializePickup(ItemToDrop->GetClass(), InQuantity);

	// Log the item properties for debugging
	UE_LOG(LogTemp, Warning, TEXT("Item dropped: ID = %s, Quantity = %d"), *DroppedItem->DesiredItemID.ToString(), DroppedItem->ItemQuantity);
}


void APickup::BeginFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(true);
	}
}

void APickup::EndFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(false);
	}
}

void APickup::Interact(ACilveks* PlayerCharacter)
{
	TakePickup(PlayerCharacter);
}





void APickup::UpdateInteractableData()
{
	InstanceInteractableData.InteractableType = EInteractableType::Pickup;
	InstanceInteractableData.Action = ItemReference->TextData.InteractionText;
	InstanceInteractableData.Name = ItemReference->TextData.Name;
	InstanceInteractableData.Quantity = ItemReference->Quantity;
	InteractableData = InstanceInteractableData;

}


void APickup::TakePickup(const ACilveks* Taker)
{
	if (!IsPendingKillPending())
	{
		if (ItemReference)
		{
			if (UInventoryComponent* PlayerInventory = Taker->GetInventory())
			{
				const FItemAddResult AddResult = PlayerInventory->HandleAddItem(ItemReference);

				switch (AddResult.OperationResult)
				{
				case EItemAddResult::IAR_NoItemAdded:
					break;
				case EItemAddResult::IAR_PartialAmountItemAdded:
					UpdateInteractableData();
					Taker->UpdateInteractionWidget();
					break;
				case EItemAddResult::IAR_AllItemAdded:
					// Print out the properties of the added item
					//PrintItemProperties(ItemReference);
					Destroy();
					break;
				}
				UE_LOG(LogTemp, Warning, TEXT("%s"), *AddResult.ResultMessage.ToString());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Player inventory component is null!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Pickup internal item reference was null!"));
		}
	}
}

void APickup::PrintItemProperties(UItemBase* Item)
{
	// Print out the properties of the item
	UE_LOG(LogTemp, Warning, TEXT("Item ID: %s"), *Item->ID.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Item Type: %s"), *UEnum::GetValueAsString(Item->ItemType));
	UE_LOG(LogTemp, Warning, TEXT("Item Quality: %d"), Item->ItemQuality);
	UE_LOG(LogTemp, Warning, TEXT("Numeric Data: %f"), Item->NumericData);
	// Get the reference to the player character
	ACilveks* PlayerCharacter = Cast<ACilveks>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (PlayerCharacter)
	{
		// Check the player character's inventory
		//CheckInventory(PlayerCharacter);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get reference to player character!"));
	}
}



void APickup::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(APickup, DesiredItemID))
	{
		if (ItemDataTable)
		{

			if (const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString()))
			{
				PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);
			}
		}
	}
}





void APickup::OnAxeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if the overlapping actor is the player character
	ACilveks* PlayerCharacter = Cast<ACilveks>(OtherActor);
	if (PlayerCharacter)
	{
		
		bAxeOverlap = true;
		//UE_LOG(LogTemp, Warning, TEXT("Overlap Start: %d"), bAxeOverlap ? 1 : 0);
		PlayerCharacter->CurrentPickup = this;
		
		
	}
	//return;


}
void APickup::OnAxeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Check if the overlapping actor is the player character
	ACilveks* PlayerCharacter = Cast<ACilveks>(OtherActor);
	if (!PlayerCharacter)
	{

		bAxeOverlap = false;
		//UE_LOG(LogTemp, Warning, TEXT("Overlap End: %d"), bAxeOverlap ? 1 : 0);
	}
	//return;


}

void APickup::BladeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ANPC* NPC = Cast<ANPC>(OtherActor);
	if (NPC)
	{
		UE_LOG(LogTemp, Warning, TEXT("BladeOverlapBegin: NPC detected"));
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("BladeOverlapBegin: PlayerAttackOverlapBegin called"));

		if (ItemReference && ItemReference->IsAxe())
		{
			UE_LOG(LogTemp, Warning, TEXT("BladeOverlapBegin: Item is an axe"));
			auto const Damage = ItemReference->GetDamage();
			auto const NewHealth = NPC->GetHealth() - Damage;
			NPC->SetHealth(NewHealth);
			UE_LOG(LogTemp, Warning, TEXT("BladeOverlapBegin: NPC health decreased by %f"), Damage);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("BladeOverlapBegin: Item is not an axe"));
		}
	}
}

void APickup::BladeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ANPC* NPC = Cast<ANPC>(OtherActor);
	if (NPC)
	{
		UE_LOG(LogTemp, Warning, TEXT("BladeOverlapEnd: PlayerAttackOverlapEnd called"));
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("BladeOverlapEnd: PlayerAttackOverlapEnd called"));
	}
}