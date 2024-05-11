
#include "World/Pickup.h" // atsauce uz header failu
#include "Speele/Cilveks.h" // atsauce uz Cilveks klasi
#include "Components/InventoryComponent.h" // atsauce uz inventoryComponent klasi
#include "Components/BoxComponent.h" // boxcomponent mijiedarbības zonas pamata Unreal engine klases atsauce
#include "Items/ItemBase.h" // atsauce uz ItemBase klasi
#include <AI_NPC/NPC.h> // atsauce uz NPC klasi
#include <Kismet/GameplayStatics.h> // atsauce uz GamePlayStatics datu atrašanas pamatklasi

// pievienotas cilveks un cilveksbase klašu references
class ACilveks;
class ACilveksBase;


APickup::APickup()
{
	// bieži atjauno priekšmetu datus lielā ātrumā, ar tick palīdzību
	PrimaryActorTick.bCanEverTick = true;

	// Izveidots mainīgais, kurš tiek nostādīts kā Mesh instance priekšmetam
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	PickupMesh->SetSimulatePhysics(true); // ieslēdz fizikas likumus priekšmetam pašā spēlē
	SetRootComponent(PickupMesh); // iestata šo mesh, kā galveno komponentu priekšmetam

	


}
// funkcija, kas nostrādā spēlei uzsākoties
void APickup::BeginPlay()
{
	Super::BeginPlay();

	// inicializē visus pickup priekšmetus, kas atrodas spēlē
	InitializePickup(UItemBase::StaticClass(), ItemQuantity);
	
	// tiek izveidota mijiedarbības zonas mainīgais, kurš satur atsauci uz to
	UBoxComponent* AxeBox = FindComponentByClass<UBoxComponent>();

	// saskarsme ar mijiedarbības zonām spēli uzsākot pēc noklusējuma tiek uzstādīta kā false
	bAxeOverlap = false;
	// ja tiek atrasta axebox mijiedarbības zona
	if (AxeBox)
	{
		// kad tiek veikta saskarsme ar mijiedarbības zonu tiek izpildīta OnAxeOverlapBegin funkcija
		AxeBox->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnAxeOverlapBegin);
		// kad saskarsme beigusies izpilda OnAxeOverlapEnd funkciju
		AxeBox->OnComponentEndOverlap.AddDynamic(this, &APickup::OnAxeOverlapEnd);
	}
	else
	{
		// testa līnija
		UE_LOG(LogTemp, Warning, TEXT("AxeBox is null!"));
	}

	// izveido atsauci uz asmens mijiedarbības zonu ar mainīgā palīdzību
	UBoxComponent* AxeBladeColBox = FindComponentByClass<UBoxComponent>();

	// ja atsauce iegūta
	if (AxeBladeColBox) {
		// kad tiek veikta saskarsme ar mijiedarbības zonu tiek izpildīta BladeOverlap funkcija
		AxeBladeColBox->OnComponentBeginOverlap.AddDynamic(this, &APickup::BladeOverlapBegin);
		// kad saskarsme beigusies izpilda BladeOverlapEnd funkciju
		AxeBladeColBox->OnComponentEndOverlap.AddDynamic(this, &APickup::BladeOverlapEnd);
	}
	else
	{
		// testa līnija
		UE_LOG(LogTemp, Warning, TEXT("AxeBladeColBox is null!"));
	}
	
}



// priekšmeta ielikšanas rokā funkcija
void APickup::Pickup(ACilveks* PlayerCharacter)
{
	// pārbauda vai spēlētāja instance pastāv
	if (!PlayerCharacter)
	{
		// testa līnija
		UE_LOG(LogTemp, Error, TEXT("Player character is not valid."));
	}

	// priekšmeta mesh instances pārbaude
	if (!PickupMesh)
	{
		UE_LOG(LogTemp, Error, TEXT("Pickup mesh is not valid."));
	}

	// ja priekšmets jau pacelts
	if (bPickedUp == true)
	{
		// izvada testa līniju un izpilda nomešanas funkciju
		UE_LOG(LogTemp, Warning, TEXT("Axe already picked up, dropping initiated"));
		DropThePickup(PlayerCharacter);
		return;
	}

	// ja tikts līdz šim solim, izvada testa līniju pacelšanas apstiprināšanai
	UE_LOG(LogTemp, Warning, TEXT("Attempting to pick up the pickup."));
	// izvada bAxeOverlap vērtību lai saprastu vai notiek mijiedarbība nospiešanas brīdī
	UE_LOG(LogTemp, Warning, TEXT("bAxeOverlap: %d"), bAxeOverlap);

	// izveido mainīgo kurš glabā atsauci uz spēlētāja mesh
	USkeletalMeshComponent* PlayerMesh = PlayerCharacter->GetMesh();
	if (PlayerMesh)
	{
		// izveido mainīgo, kurš glabās priekšmeta Socket nosaukumu (locītava, kurā ievietots priekšmets)
		FName AttachedSocket = PickupMesh->GetAttachSocketName();
		// ja locītava ir aizņemta un attachedsocket ir kāda vērtība
		if (PlayerMesh->DoesSocketExist(AttachedSocket))
		{
			// izvada testa līniju ka kaut kas jau atrodas locītavā un tiek palaista nomešanas funkcija
			UE_LOG(LogTemp, Warning, TEXT("Something is already attached to AxeSocket."));
			DropThePickup(PlayerCharacter);
			return;
		}
	}
	// ja notiek mijiedarbība ar zonu
	if (bAxeOverlap)
	{
		
		// pievieno priekšmeta mesh spēlētāja mesham Unreal Engine vidē izveidotajā  AxeSocket locītavā
		PickupMesh->AttachToComponent(PlayerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "AxeSocket");

		// liek izslēgt fizikas likumus lai netraucētu spēlētāja kustības
		PickupMesh->SetSimulatePhysics(false);
		// nosaka ka priekšmets ir paņemts rokā
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
	// pārbauda vai ir atsauce uz spēlētāju
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("Player character is not valid."));
		
	}
	UE_LOG(LogTemp, Error, TEXT("DROPTHEPICKUP"));
	// atdala priekšmetu no spēlētāja rokas
	PickupMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	// ievieš parastos fizikas likumus
	PickupMesh->SetSimulatePhysics(true);
	// nosaka ka priekšmets nav pacelts
	bPickedUp = false;
}

void APickup::InitializeNpcDrop(UDataTable* InItemDataTable, const FName& InItemDataTableRowName, int32 InQuantity)
{
	// nosaka datu tabulu, priekšmeta daudzumu un ID priekš nomestā priekšmeta
	ItemDataTable = InItemDataTable;
	ItemQuantity = InQuantity;
	DesiredItemID = InItemDataTableRowName;

	// pārbauda vai pastāv datu tabula
	if (ItemDataTable)
	{
		// iegūst datus no datu tabulas ar instances palīdzību
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());
		if (ItemData)
		{
			// izveido jaunu priekšmeta referenci
			ItemReference = NewObject<UItemBase>(this);

			// nosaka priekšmeta vērtības no tabulas rindas datiem
			ItemReference->ID = ItemData->ID;
			ItemReference->ItemType = ItemData->ItemType;
			ItemReference->ItemQuality = ItemData->ItemQuality;
			ItemReference->NumericData = ItemData->NumericData;
			ItemReference->TextData = ItemData->TextData;
			ItemReference->AssetData = ItemData->AssetData;

			// nosaka priekšmeta daudzumu
			InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);

			// iestata Mesh izmantojot assetdata datus no itemdata
			if (PickupMesh)
			{
				PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);
			}

			// tiek atjaunoti priekšmeta Interactable dati
			UpdateInteractableData();
		}
		else
		{
			// testa līnija, ja itemdata ir tukšs
			UE_LOG(LogTemp, Warning, TEXT("Failed to find ItemData for DesiredItemID: %s"), *DesiredItemID.ToString());
		}
	}
	else
	{
		// testa līnija, ja itemdatatable ir tukšs
		UE_LOG(LogTemp, Error, TEXT("Item data table is invalid."));
	}
}


// inicializē pickup objektu, iestatot tā pamatklases tipu un kvantitāti
void APickup::InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity)
{
	// ja pastāv datu tabulas atsauce un ir derīgs DesiredItemID
	if (ItemDataTable && !DesiredItemID.IsNone())// !
	{
		// itemdata klases mainīgajā tiek saglabāti dati no datu tabulas rindas
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());
		if (ItemData)
		{
			// pārbauda vai priekšmeta reference jau pastāv
			if (!ItemReference)
			{
				// izveido jaunu priekšmeta referenci
				ItemReference = NewObject<UItemBase>(this, BaseClass);
			}
			// nosaka priekšmeta vērtības no tabulas rindas datiem
			ItemReference->ID = ItemData->ID;
			ItemReference->ItemType = ItemData->ItemType;
			ItemReference->ItemQuality = ItemData->ItemQuality;
			ItemReference->NumericData = ItemData->NumericData;
			ItemReference->TextData = ItemData->TextData;
			ItemReference->AssetData = ItemData->AssetData;
			// nosaka priekšmeta daudzumu
			InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);

			// inicializē priekšmetu ar datiem, kas atrodami datu tabulā norādītajam desireditemID ID
			ItemReference->InitializeFromDataTable(DesiredItemID, ItemDataTable);
			// iestata Mesh izmantojot assetdata datus no itemdata mainīgā
			PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);
			// tiek atjaunoti priekšmeta interactable dati
			UpdateInteractableData();
		}
		else
		{
			// testa līnija
			UE_LOG(LogTemp, Warning, TEXT("Failed to find ItemData for DesiredItemID: %s"), *DesiredItemID.ToString());
		}
	}
}

void APickup::InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity, UDataTable* InItemDataTable)
{	
	// ja ir pieejama atsauce uz priekšmetu ko izmest no inventory
	if (!ItemToDrop)
	{
		// testa līnija pasaka ka atsauce nenorāda uz nevienu priekšmetu
		UE_LOG(LogTemp, Error, TEXT("ItemToDrop is nullptr in InitializeDrop"));
		return;
	}

	// Iegūst PickupBase blueprint klasi, kuru izmanto objektu novietošanai spēles pasaulē
	UClass* BP_PickupBaseClass = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Game/Blueprints/BP_PickupBase.BP_PickupBase_C"));

	// ja nav iegūta atsauce uz blueprint objekta klasi, tiek pārtraukta funkcija
	if (!BP_PickupBaseClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load BP_PickupBase blueprint class"));
		return;
	}

	// izveido jaunu PickupBase Blueprinta objekta instanci, kuru ievietot spēles pasaulē
	APickup* DroppedItem = GetWorld()->SpawnActor<APickup>(BP_PickupBaseClass, GetActorTransform());
	// ja atsauce uz izveidoto instanci ir tukša, beigt funkciju
	if (!DroppedItem)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn dropped item"));
		return;
	}

	// nomestajam priekšmetam piešķir ID, daudzumu un datu tabulu
	DroppedItem->DesiredItemID = ItemToDrop->ID;
	DroppedItem->ItemQuantity = InQuantity;
	DroppedItem->ItemDataTable = InItemDataTable;

	// inicializēt pickup priekšmetu ar nokopētajām vērtībām
	DroppedItem->InitializePickup(ItemToDrop->GetClass(), InQuantity);

	// testa līnija
	UE_LOG(LogTemp, Warning, TEXT("Item dropped: ID = %s, Quantity = %d"), *DroppedItem->DesiredItemID.ToString(), DroppedItem->ItemQuantity);
}

// funkcija priekš fokusēšanās uz pickup priekšmetu
void APickup::BeginFocus()
{
	// ja ir derīga pickupmesh instance
	if (PickupMesh)
	{
		// vizuāli izceļ priekšmeta mesh
		PickupMesh->SetRenderCustomDepth(true);
	}
}
// fokusēšanās beigšanas funkcija
void APickup::EndFocus()
{
	if (PickupMesh)
	{
		// beidz vizuāli izcelt priekšmeta mesh
		PickupMesh->SetRenderCustomDepth(false);
	}
}
// mijiedarbības funkcija kas nostrādā, kad ir fokusēts priekšmets un nospiesta poga
void APickup::Interact(ACilveks* PlayerCharacter) // saņem referenci uz spēlētāja varoni
{
	// izpilda pickup priekšmeta ielikšanas rokā funkciju
	TakePickup(PlayerCharacter);
}




// atjauno datus priekš interaction logrīka
void APickup::UpdateInteractableData()
{
	InstanceInteractableData.InteractableType = EInteractableType::Pickup;
	InstanceInteractableData.Action = ItemReference->TextData.InteractionText;
	InstanceInteractableData.Name = ItemReference->TextData.Name;
	InstanceInteractableData.Quantity = ItemReference->Quantity;
	// piešķir vērtības InteractableData struktūras mainīgajam
	InteractableData = InstanceInteractableData;

}

// funkcija priekš priekšmeta ievietošanas inventory
void APickup::TakePickup(const ACilveks* Taker)
{
	// pārbauda vai priekšmets jau nav paredzēts iznīcināšanai
	if (!IsPendingKillPending())
	{
		if (ItemReference)
		{
			// ja var iegūt atsauci uz spēlētāja inventory
			if (UInventoryComponent* PlayerInventory = Taker->GetInventory())
			{
				// izveido mainīgo, kurš saturēs priekšmeta pievienošanas inventory rezultātu
				const FItemAddResult AddResult = PlayerInventory->HandleAddItem(ItemReference);

				// switch ar visiem pieejamajiem variantiem
				switch (AddResult.OperationResult)
				{
					// priekšmets netika pievienots inventory
				case EItemAddResult::IAR_NoItemAdded:
					break;
					// tikai daļu priekšmeta pievienoja inventory
				case EItemAddResult::IAR_PartialAmountItemAdded:
					// atjauno datus priekš interaction logrīka 
					UpdateInteractableData();
					// ar cilvēka atsauci atjauno interakcijas logrīka datus
					Taker->UpdateInteractionWidget();
					break;
					// kad viss priekšmeta skaits pievienots, izdzēš fizisko priekšmetu kas atradās pasaulē
				case EItemAddResult::IAR_AllItemAdded:
					Destroy();
					break;
				}
				// izvada rezultātu izvades logā
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
	// izvada priekšmeta vērtības izvades logā
	UE_LOG(LogTemp, Warning, TEXT("Item ID: %s"), *Item->ID.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Item Type: %s"), *UEnum::GetValueAsString(Item->ItemType));
	UE_LOG(LogTemp, Warning, TEXT("Item Quality: %d"), Item->ItemQuality);
	UE_LOG(LogTemp, Warning, TEXT("Numeric Data: %f"), Item->NumericData);
}


// nostrādā kad Unreal Engine vidē izmainītas priekšmeta vērtības
void APickup::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	// izpilda pamata funkciju
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// tiek inicializēts changedpropertyname, kas ir jaunā izmainītā vērtība, ja nav mainījusies vērtība, tad tiek saglabāts ar NAME_None
	const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	// ja changedpropertyname vērtība sakrīt ar DesiredItemID vērtību
	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(APickup, DesiredItemID))
	{
		// ja ir derīga atsauce uz ItemDataTable
		if (ItemDataTable)
		{
			// ja pastāv rinda ar DesiredItemID ID
			if (const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString()))
			{
				// piešķir priekšmetam šīs rindas noteikto mesh
				PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);
			}
		}
	}
}




// funkcija ar vajadzīgajiem parametriem mijiedarbības noteikšanai
void APickup::OnAxeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// pārbauda vai mijiedarbojās tieši ar spēlētāju
	ACilveks* PlayerCharacter = Cast<ACilveks>(OtherActor);
	if (PlayerCharacter)
	{
		bAxeOverlap = true;
		
		// pickup priekšmets tiek noteikts kā pašreizējais pickup priekšmets, ar kuru notiek interakcija
		PlayerCharacter->CurrentPickup = this;
		
		
	}
}
// funkcija ar vajadzīgajiem parametriem mijiedarbības noteikšanai
void APickup::OnAxeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// pārbauda vai mijiedarbojās tieši ar spēlētāju
	ACilveks* PlayerCharacter = Cast<ACilveks>(OtherActor);
	// ja playercharacter nav vērtības
	if (!PlayerCharacter)
	{
		bAxeOverlap = false;// nosaka mijiedarbības vērtību kā false
	}
}

// funkcija ar vajadzīgajiem parametriem mijiedarbības noteikšanai
void APickup::BladeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// pārbauda vai notiek mijiedarbība ar NPC varoni
	ANPC* NPC = Cast<ANPC>(OtherActor);
	// ja iegūta saskarsme ar NPC
	if (NPC)
	{
		UE_LOG(LogTemp, Warning, TEXT("BladeOverlapBegin: NPC detected"));

		// izvada ierakstu uz paša ekrāna, nevis izvades logā, testa opcijai
		//if (GEngine)
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("BladeOverlapBegin: PlayerAttackOverlapBegin called"));

		// ja ir priekšmeta reference un tas ir ierocis (kategorija weapon)
		if (ItemReference && ItemReference->IsAxe())
		{
			UE_LOG(LogTemp, Warning, TEXT("BladeOverlapBegin: Item is an axe"));
			// izveido bojājumu mainīgo, kurš saturēs priekšmeta damagevalue vērtību
			auto const Damage = ItemReference->GetDamage();
			// izveido jauno dzīvību mainīgo kurš glabās jauno npc dzīvību vērtību pēc to noņemšanas
			auto const NewHealth = NPC->GetHealth() - Damage;
			// iestata Npc dzīvību vērtību par mainīgā vērtību
			NPC->SetHealth(NewHealth);

			UE_LOG(LogTemp, Warning, TEXT("BladeOverlapBegin: NPC health decreased by %f"), Damage);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("BladeOverlapBegin: Item is not an axe"));
		}
	}
}
// funkcija ar vajadzīgajiem parametriem mijiedarbības noteikšanai
void APickup::BladeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// pārbauda vai notiek mijiedarbība ar NPC varoni
	ANPC* NPC = Cast<ANPC>(OtherActor);
	if (!NPC)
	{
		UE_LOG(LogTemp, Warning, TEXT("BladeOverlapEnd: PlayerAttackOverlapEnd called"));
		//if (GEngine)
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("BladeOverlapEnd: PlayerAttackOverlapEnd called"));
	}
}