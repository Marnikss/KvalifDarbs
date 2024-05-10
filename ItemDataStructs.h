#pragma once // domats lai noverstu liekas atsauces citos failos

#include "CoreMinimal.h" // nodrosina ka ir ieklauta tikai minimala pamata funkcionalitate kas var palidzet samazinat kompilacijas laiku
#include "Engine/DataTable.h" // atsauce uz failu kas nodrosina iespeju stradat ar datu tabulam data tables Unreal Engine projektos
#include "ItemDataStructs.generated.h" // ietver generetu C++ kodu kas nodrosina mijiedarbibu ar blueprintiem uc

USTRUCT(BlueprintType) // struktura pieejama blueprint skriptiem
struct SPEELE_API FItemDataStructs // struktura ir dala no SPEELE Modula
{

public:
	GENERATED_BODY() // genere nepieciesamo kodu Engine Sistemai

	FItemDataStructs(); // Konstruktors kas tiek izsaukts kad tiek izveidots jauns FItemDataStructs objekts
	~FItemDataStructs(); // Destruktors kas tiek izsaukts kad tiek iznicinats FItemDataStructs objekts
};
UENUM() // pieejams Blueprint skriptiem un to var izmantot Blueprint klases vai objekta definicija
enum class EItemQuality : uint8 // Define uzskaites tipu EItemQuality kuram ir 8 bitu izmers un kurs ir definets ka klase
{
	// Define visas prieksmetu kvalitates un iedod tam nosaukumus kurus tie uzradis spele
	Common UMETA(DisplayName = "Common"),
	Uncommon UMETA(DisplayName = "Uncommon"),
	Rare UMETA(DisplayName = "Rare"),
	Epic UMETA(DisplayName = "Epic"),
	Legendary UMETA(DisplayName = "Legendary")
};

UENUM() // pieejams Blueprint skriptiem un to var izmantot Blueprint klases vai objekta definicija
enum class EItemType : uint8 // Define uzskaites tipu EItemType kuram ir 8 bitu izmers un kurs ir definets ka klase
{
	// Define visus prieksmetu tipus un iedod tiem nosaukumus kurus tie uzradis spele
	Armor UMETA(DisplayName = "Armor"),
	Weapon UMETA(DisplayName = "Weapon"),
	Consumable UMETA(DisplayName = "Consumable"),
	Resource UMETA(DisplayName = "Resource")

};

USTRUCT(BlueprintType) // struktura pieejama blueprint skriptiem
struct FItemStatistics // define strukturu kas satures datus par siem prieksmetu aspektiem
{
	GENERATED_BODY() // genere nepieciesamo kodu Engine Sistemai

	UPROPERTY(EditAnywhere) // ipasiba ir redigejama jebkura vieta tai skaita ari redaktora
	float ArmorRating;

	UPROPERTY(EditAnywhere) // ipasiba ir redigejama jebkura vieta tai skaita ari redaktora
	float DamageValue;

	UPROPERTY(EditAnywhere) // ipasiba ir redigejama jebkura vieta tai skaita ari redaktora
	float HealingAmount;

	UPROPERTY(EditAnywhere) // ipasiba ir redigejama jebkura vieta tai skaita ari redaktora
	float SellValue;
};

USTRUCT(BlueprintType) // struktura pieejama blueprint skriptiem
struct FItemTextData // define strukturu kas satures prieksmetu teksta datus
{
	GENERATED_BODY() // genere nepieciesamo kodu Engine Sistemai

	UPROPERTY(EditAnywhere) // ipasiba ir redigejama jebkura vieta tai skaita ari redaktora
	FText Name;

	UPROPERTY(EditAnywhere) // ipasiba ir redigejama jebkura vieta tai skaita ari redaktora
	FText Description;

	UPROPERTY(EditAnywhere) // ipasiba ir redigejama jebkura vieta tai skaita ari redaktora
	FText InteractionText;

	UPROPERTY(EditAnywhere) // ipasiba ir redigejama jebkura vieta tai skaita ari redaktora
	FText UsageText;
};

USTRUCT(BlueprintType) // struktura pieejama blueprint skriptiem
struct FItemNumericData // define strukturu kas satures prieksmetu numeriskos datus
{
	GENERATED_BODY() // genere nepieciesamo kodu Engine Sistemai

	UPROPERTY(EditAnywhere) // ipasiba ir redigejama jebkura vieta tai skaita ari redaktora
	float Weight;

	UPROPERTY(EditAnywhere) // ipasiba ir redigejama jebkura vieta tai skaita ari redaktora
	int32 MaxStackSize;

	UPROPERTY(EditAnywhere) // ipasiba ir redigejama jebkura vieta tai skaita ari redaktora
	bool bIsStackable;
};

USTRUCT(BlueprintType) // struktura pieejama blueprint skriptiem
struct FItemAssetData // struktura kas satur datus par prieksmetu ikonam un Character Mesh
{
	GENERATED_BODY() // genere nepieciesamo kodu Engine Sistemai

	UPROPERTY(EditAnywhere) // ipasiba ir redigejama jebkura vieta tai skaita ari redaktora
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere) // ipasiba ir redigejama jebkura vieta tai skaita ari redaktora
	UStaticMesh* Mesh;
};

USTRUCT(BlueprintType) // struktura pieejama blueprint skriptiem
struct FItemData : public FTableRowBase // define FItemData strukturu kas atbilst vienai rindinai datu
//tabula kas satur informaciju par konkretu prieksmetu spele
{
	GENERATED_BODY() // genere nepieciesamo kodu Engine Sistemai

	UPROPERTY(EditAnywhere, Category = "Item Data") // ipasiba ir redigejama jebkura vieta tai skaita ari redaktora   kategorija ir Item Data
	FName ID;

	UPROPERTY(EditAnywhere, Category = "Item Data") // ipasiba ir redigejama jebkura vieta tai skaita ari redaktora   kategorija ir Item Data
	EItemQuality ItemQuality;

	UPROPERTY(EditAnywhere, Category = "Item Data") // ipasiba ir redigejama jebkura vieta tai skaita ari redaktora   kategorija ir Item Data
	EItemType ItemType;

	UPROPERTY(EditAnywhere, Category = "Item Data") // ipasiba ir redigejama jebkura vieta tai skaita ari redaktora   kategorija ir Item Data
	FItemStatistics ItemStatistics;

	UPROPERTY(EditAnywhere, Category = "Item Data") // ipasiba ir redigejama jebkura vieta tai skaita ari redaktora   kategorija ir Item Data
	FItemTextData TextData;

	UPROPERTY(EditAnywhere, Category = "Item Data") // ipasiba ir redigejama jebkura vieta tai skaita ari redaktora   kategorija ir Item Data
	FItemNumericData NumericData;

	UPROPERTY(EditAnywhere, Category = "Item Data") // ipasiba ir redigejama jebkura vieta tai skaita ari redaktora   kategorija ir Item Data
	FItemAssetData AssetData;


};

