#pragma once // domāts lai novērstu liekas atsauces citos failos

#include "CoreMinimal.h" // nodrošina, ka ir iekļauta tikai minimālā pamata funkcionalitāte, kas var palīdzēt samazināt kompilācijas laiku
#include "GameFramework/Actor.h" // Iekļauj nepieciešamo Actor objekta pamata klasi
#include "Interfaces/InteractionInterface.h" // atsauce uz InteractionInterface klasi
#include <Components/BoxComponent.h> // iekļauj boxcomponent pamata klases funkcionalitāti
#include "Pickup.generated.h" // ietver generetu C++ kodu kas nodrosina mijiedarbibu ar blueprintiem uc

// atsauces uz klasēm priekš to atpazīšanas, deklarācijas definē, ka šīs klases ir nepieciešamas šajā failā
class UDataTable;
class UItemBase;
class ACilveks;
class UInventoryComponent;

UCLASS() // norāda, ka šī klase ir Unreal Engine klase
class SPEELE_API APickup : public AActor, public IInteractionInterface // pickup klase manto AActor un InteractionInterface klašu funkcionalitāti
{
	GENERATED_BODY() // pievieno nepieciešamās funkcijas un īpašības, lai padarītu šo klasi derīgu lietošanai
	
public:	

	//===================================================
	// FUNKCIJAS
	//===================================================

	APickup(); // klases konstruktora deklarācija

	UFUNCTION(BlueprintCallable, Category = "Pickup") // BlueprintCallable funkcija, kas nozīmē, ka to var izsaukt no Blueprint skriptiem Unreal Engine vidē
	void Pickup(ACilveks* PlayerCharacter); // funkcija priekš objekta ielikšanas rokā

	void DropThePickup(ACilveks* PlayerCharacter); // funkcija priekš priekšmeta nomešanas

	void InitializeNpcDrop(UDataTable* InItemDataTable, const FName& InItemDataTableRowName, int32 InQuantity); // funkcija priekš npc priekšmeta nomešanas

	void InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity); // funkcija kas iestatīs priekšmeta vērtības iekš inventory

	void InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity, UDataTable* InItemDataTable); // iestata izmestā priekšmeta vērtības

	UFUNCTION() //nodrošina funkcijas savienojumu starp C++ kodu un Unreal Engine sistēmas sastāvdaļām
	// funkcija, kas atbild par cirvja pacelšanas mijiedarbības zonas nostrādāšanu un funkcionalitāti
	void OnAxeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION() //nodrošina funkcijas savienojumu starp C++ kodu un Unreal Engine sistēmas sastāvdaļām
	// funkcija, kas atbild par cirvja pacelšanas mijiedarbības zonas nostrādāšanu un funkcionalitāti
	void OnAxeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION() //nodrošina funkcijas savienojumu starp C++ kodu un Unreal Engine sistēmas sastāvdaļām
	// funkcija, kas atbild par cirvja asmens mijiedarbības zonas nostrādāšanu un funkcionalitāti
	void BladeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() //nodrošina funkcijas savienojumu starp C++ kodu un Unreal Engine sistēmas sastāvdaļām
	// funkcija, kas atbild par cirvja asmens mijiedarbības zonas nostrādāšanu un funkcionalitāti
	void BladeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// forceinline izsauc funkciju bez liekas deklarēšanas cpp failā
	FORCEINLINE UItemBase* GetItemData() { return ItemReference; }; // atgriež atsauci uz objektu, ar kuru mijiedarbojās

	virtual void BeginFocus() override; // uzsāk fokusu uz priekšmetu, kuru iespējams pacelt
	virtual void EndFocus() override; // beidz fokusu uz priekšmetu, kuru iespējams pacelt

	virtual void BeginPlay() override; // beginplay funkcija kas izpildās spēlei sākoties

	virtual void Interact(ACilveks* PlayerCharacter) override; // izpilda TakePickup funkciju kad izpildās

	void UpdateInteractableData(); // piešķir priekšmetam datus

	void TakePickup(const ACilveks* Taker); // ievieto pickup priekšmetu inventory

	void PrintItemProperties(UItemBase* Item); // funkcija kas izprintē priekšmeta vērtības testa mērķiem

	// funkciju izsauc ja Unreal Engine vidē tiek mainīta kāda no priekšmeta vērtībām
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	 //===================================================
	// MAINĪGIE un ATRIBŪTI
	//===================================================

	// mainīgais kas nosaka vai priekšmets atrodas spēlētāja rokā
	bool bPickedUp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	bool bAxeOverlap; // palīdz noteikt vai pašlaik strādā cirvja mijiedarbības zonas funkcionalitāte

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemData")
	TSubclassOf<class UItemBase> ItemBaseClass; // iegūst atsauci uz ItemBase klasi, kura nosaka priekšmetu vērtību veidus

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Components")
	UStaticMeshComponent* PickupMesh; // atsauce uz priekšmeta Unreal Engine vidē uzstādīto Mesh

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	UDataTable* ItemDataTable; // atsauce uz priekšmetu datu tabulu

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	FName DesiredItemID; // priekšmeta ID mainīgais Fname formātā

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Item Reference")
	UItemBase* ItemReference; // mainīgais, kuru izmantos lai iegūtu atsauci uz ItemBase klasi un tās vērtībām

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	int32 ItemQuantity; // mainīgais kas saturēs priekšmeta daudzumu

	UPROPERTY(VisibleInstanceOnly, Category = "Pickup | Interaction")
	FInteractableData InstanceInteractableData; // ar mainīgo izveido instanci, kas manto šīs struktūras datus
};
