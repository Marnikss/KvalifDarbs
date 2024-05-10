#pragma once // domāts lai novērstu liekas atsauces citos failos
#include "GameFramework/CharacterMovementComponent.h" // Engine faila atsauce kas atbild par spēlētāja kustības funkcijām
#include <Interfaces/InteractionInterface.h> // Atsauce uz InteractionInterface klasi
#include "CoreMinimal.h" // nodrošina, ka ir iekļauta tikai minimālā pamata funkcionalitāte, kas var palīdzēt samazināt kompilācijas laiku
#include "GameFramework/Character.h" // atsauce uz Unreal engine base Character klasi, kas atbild par spēlētāja pamata funkcionalitāti
#include "GameFramework/SpringArmComponent.h" // atsauce uz Springarmcomponent, kas atbild par kameras kustību
#include "Camera/CameraComponent.h" // atsauce uz kameras komponentu
#include <Components/InventoryComponent.h> // atsauce uz InventoryComponent klasi
#include "Animation/AnimMontage.h" // atsauce uz animmontage klasi, kas atbild par animācijām un to palaišanu
#include "AI_NPC/CilveksBase.h" // atsauce uz npc cilveksbase klasi
#include <Components/WidgetComponent.h> // atsauce uz widgetcomponent klasi, atbild par logrīkiem

#include "Cilveks.generated.h" // ietver generetu C++ kodu kas nodrosina mijiedarbibu ar blueprintiem uc


// pasludināšana ka šīs klases eksistē, atribūtu atpazīšanai
class ACilvekaHUD; 
class UInventoryComponent;
class UItemBase;
class APickup;


USTRUCT() // padara struktūru saderīgu ar Unreal engine
struct FInteractionData // struktūra kura glabā informāciju par pašreizējo priekšmetu ar kuru 
//mijiedarbojās spēlētājs un pēdējo mijiedarbības brīdi un laiku kad tas noticis
{
    GENERATED_BODY() // Ģenerē nepieciešamo kodu Engine sistēmai, lai Unreal varētu apstrādāt šo kodu

    // konstruktors, kurā tiek inicializēti pašreizējais priekšmets un pēdējā mijiedarbības brīža laiks
    FInteractionData() : CurrentInteractable(nullptr), LastInteractionCheckTime(0.0f)
    {

    };

    UPROPERTY() // nodrošina, ka šis mainīgais būs redzams un pieejams citiem 
    //Unreal Engine moduļiem un sistēmas daļām
    AActor* CurrentInteractable;

    UPROPERTY() // nodrošina, ka šis mainīgais būs redzams un pieejams citiem 
    //Unreal Engine moduļiem un sistēmas daļām
    float LastInteractionCheckTime;
};

UCLASS() // klasi padara saderīgu ar Unreal engine sistēmu
class SPEELE_API ACilveks : public ACharacter // definēta ACilveks klase, kura mantota no ACharacter Unreal engine
// bāzes klases
{
    GENERATED_BODY() // Ģenerē nepieciešamo kodu Engine sistēmai, lai Unreal varētu apstrādāt šo kodu

 public:

     //===================================================
    // FUNKCIJAS
    //===================================================

     ACilveks(); // cilvēka klases konstruktors

     // FORCEINLINE liek funkcijai izpildīties tieši šeit
     // pārbauda vai mijiedarbības taimeris ir aktīvs (TimerHandle_Interaction), ja ir aktīvs atgriež true
     FORCEINLINE bool IsInteracting() const {
         return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction);
     }

     // FORCEINLINE liek funkcijai izpildīties tieši šeit
     // atgriež norādi uz UInventoryComponent objektu, kas tiek saglabāta mainīgajā PlayerInventory
     FORCEINLINE UInventoryComponent* GetInventory() const { 
         return PlayerInventory; 
     }

     // Funkcija kas atbild par priekšmetu pacelšanas logrīka saturu
     void UpdateInteractionWidget() const;

     // Funkcija kas izmet no Inventory atlasīto priekšmetu
     void DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop);

     float GetHealth() const; // Funkcija kas iegūst spēlētāja dzīvības
     float GetMaxHealth() const; // Funkcija kas iegūst spēlētāja maksimālās dzīvības
     void SetHealth(float const NewHealth); // Funkcija kas nosaka spēlētāja dzīvības

     //===================================================
    // MAINĪGIE un ATRIBŪTI
    //===================================================

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory") // var rediģēt gan no C++ koda, gan 
    // paša editora, Blueprintos var tikai lasīt, piešķir kategoriju

    // definē mainīgo, kurš kalpos kā atsauce uz inventoryComponent klasi
    class UInventoryComponent* InventoryComponent; 

    // definē mainīgo, kurš kalpos kā atsauce uz Pickup klasi
    class APickup* CurrentPickup;

    // definē mainīgo, kurš kalpos kā atsauce uz TradingCenter klasi
    class ATradingCenter* CurrentCenter;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Axe")// var rediģēt gan no C++ koda, gan 
    // paša editora, Blueprintos var tikai lasīt, piešķir kategoriju
    bool bAxeEquipped; // boolean mainīgais kas pārbauda vai cirvis ir spēlētāja rokā


    

    UPROPERTY(VisibleAnywhere, Category = "Character | Inventory ")// var rediģēt gan no C++ koda, gan 
    // paša editora, piešķir kategoriju
    UInventoryComponent* PlayerInventory; // Mainīgais kas glabā atsauci uz Inventorycomponent klasi, kā pats inventory

    

 protected: // klases locekļi, kas ir pieejami tikai pašai klasei un tās atvasinātajām klasēm

     //===================================================
     // FUNKCIJAS
     //===================================================
    
     // funkcija ka atbild par spēlētāja nospiestajām pogām un funkciju izpildīšanos pēc katras nospiestās pogas
     virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

     //===================================================
    // MAINĪGIE un ATRIBŪTI
    //===================================================

     // boolean mainīgais kas atbild par to vai notiek fokuss uz priekšmetu
     bool bIsInteracting = false;

     // atsauce uz priekšmetu datu tabulu
     UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")// var rediģēt gan no C++ koda, gan 
     // paša editora,var lasīt blueprintos, piešķir kategoriju
     class UDataTable* MyItemDataTable; // mainīgais kas ietver atsauci uz datu dabulu

     

     UPROPERTY()// nodrošina, ka šis mainīgais būs redzams un pieejams citiem 
    //Unreal Engine moduļiem un sistēmas daļām
     ACilvekaHUD* HUD; //mainīgais kas satur atsauci uz ACilvekaHUD

    UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")// var rediģēt gan no C++ koda, gan 
     // paša editora, piešķir kategoriju
    TScriptInterface<IInteractionInterface> TargetInteractable; // Mainīgais kas glabā atsauci uz InteractionInterface

    // float mainīgais kas norāda cik bieži notiek mijiedarbības pārbaude
    float InteractionCheckFrequency;

    // float mainīgais kas norāda mijiedarbības maksimālo attālumu
    float InteractionCheckDistance;

    // struktūra no Unreal engine, kas izveido un pārvalda taimerus
    FTimerHandle TimerHandle_Interaction; // mainīgais kas turēs taimera laiku


    FInteractionData InteractionData; // mainīgais kas glabā atsauci uz FInteractionData struktūru un tās locekļiem

    
 private: // klases locekļi, kas ir pieejami tikai pašai klasei

     //===================================================
     // FUNKCIJAS
     //===================================================

     // Parāda un slēpj inventory menu paneli
     void ToggleMenu();

     // funkcija kas atbild par priekšmeta pamanīšanu un fokusēšanos uz to
     void PerformInteractionCheck();

     // ieslēdz interaction logrīku kad atrasts un fokusēts priekšmets
     void FoundInteractable(AActor* NewInteractable);

     // paslēpj interaction logrīku kad vairs netiek fokusēts uz priekšmetu
     void NoInteractableFound();

     // ja mijiedarbība notiek ar 0.1 sekundes biežumu, izsauc interact
     void BeginInteract();

     // Notīra taimeri, kas tika iestatīts, lai veiktu mijiedarbību ar kādu objektu.
     //Beidz mijiedarbību ar mērķa objektu, ja tas ir derīgs.
     void EndInteract();

     // nosaka spēlētāju kurš veicis mijiedarbību
     void Interact();

     // funkcija kas sākas kad iesākta spēle
     virtual void BeginPlay() override;

     // funkcija, kuras mērķis ir atjaunināt spēles stāvokli katrā kadra laikā.
     virtual void Tick(float DeltaSeconds) override;

     // funkcija priekš spēlētāja kustības uz priekšu
     void MoveForward(float Value);

     // funkcija priekš spēlētāja kustības uz pa labi un pa kreisi
     void MoveRight(float Value);

     // funkcija priekš spēlētāja griešanās ar peles kustību palīdzību
     void Turn(float Value);

     // funkcija priekš spēlētāja skatīšanās uz augšu un leju
     void LookUp(float Value);

     // funkcija priekš spēlētāja lekšanas
     void StartJump();

     // funkcija priekš spēlētāja lekšanas beigšanas
     void StopJump();

     // funkcija priekš spēlētāja kustības uz atpakaļu
     void WalkBackward(float Value);

     // funkcija priekš spēlētāja tupšanās
     void ToggleCrouch(float Value);

     // funkcijas priekš priekšmeta ielikšanas spēlētāja rokā
     void OnPickupPressed();
     void OnPickupReleased();

     // funkcija priekš spēlētāja sprinta kustības
     void StartSprint();
     void StopSprint();

     // funkcija priekš NPC spēlētāja jušanas
     void SetupStimulusSource();

     // funkcija priekš spēlētāja uzbrukšanas animācijas izspēlēšanas
     void OnAttack();

     // funkcija priekš trading panel logrīka parādīšanas
     void ToggleTradingPanel();

     // funkcija priekš startscreenwidget logrīka parādīšanas
     void ToggleStartScreenWidget();


     //===================================================
    // MAINĪGIE un ATRIBŪTI
    //===================================================

     UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))// var rediģēt gan no C++ koda, gan 
    // paša editora,blueprintos var lasīt, piešķir kategoriju un var piekļūt par ja vērtība ir privāta
     UAnimMontage* Montage; // mainīgais satur animācijas montāžas atsauci

     class UAIPerceptionStimuliSourceComponent* StimulusSource; // mainīgais ar kura palīdzību var piekļūt AIPerceptionStimuliSourceComponent funkcionalitātei

     class UWidgetComponent* WidgetComponent; // mainīgais satur logrīka atsauci

     float const MaxHealth{ 100.f }; // tiek inicializēta dzīvību maksimālā vērtība
     float Health; // satur dzīvību vērtību

    bool bIsSprinting; // mainīgais nosaka vai tiek sprintots vai nē

    UPROPERTY(VisibleAnywhere)// var rediģēt gan no C++ koda, gan paša editora
    USpringArmComponent* SpringArmComponent; // atsauce uz komponentu kas domāts kameras pozicionēšanai

    UPROPERTY(VisibleAnywhere)// var rediģēt gan no C++ koda, gan paša editora
    UCameraComponent* CameraComponent; // atsauce uz pašas kameras komponentu

    
    
    
    
};
