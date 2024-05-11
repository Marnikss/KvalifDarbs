#pragma once // domāts lai novērstu liekas atsauces citos failos

#include "CoreMinimal.h" // nodrošina, ka ir iekļauta tikai minimālā pamata funkcionalitāte, kas var palīdzēt samazināt kompilācijas laiku
#include "GameFramework/SaveGame.h" // Engine faila atsauce kas atbild par spēlētāja kustības funkcijām
#include "SpeeleSaveGame.generated.h" // ietver generetu C++ kodu kas nodrosina mijiedarbibu ar blueprintiem uc

USTRUCT(BlueprintType) // struktūra ir redzama un pieejama Blueprint klases skriptos
struct FInventoryItem // Definē struktūru
{
    GENERATED_BODY() // pievieno nepieciešamās funkcijas un īpašības, lai padarītu šo struktūru derīgu

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemName; // Fstring formāta mainīgais kas saturēs priekšmeta nosaukumu
};

UCLASS() // norāda, ka šī klase ir Unreal Engine klase
class SPEELE_API USpeeleSaveGame : public USaveGame // izveidotā klase manto Unreal Engine spēļu saglabāšanas pamatklasi
{
    GENERATED_BODY() // pievieno nepieciešamās funkcijas un īpašības, lai padarītu šo struktūru derīgu

public:
    USpeeleSaveGame(); // konstruktors

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FVector PlayerPosition; // vektora mainīgais, kas glabās spēlētāja pozīciju

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TArray<FInventoryItem> Inventory; // masīva mainīgais kas saturēs Inventory satura info
};
