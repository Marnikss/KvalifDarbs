// SpeeleGameInstance.cpp

#include "SpeeleGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include <Speele/Cilveks.h>
#include <AI_NPC/NPC.h>
#include <Items/ItemSpawner.h>

USpeeleGameInstance::USpeeleGameInstance()
{
    
}

// USpeeleGameInstance.cpp
// USpeeleGameInstance.cpp
void USpeeleGameInstance::SaveGame()
{
    // Create a save game object
    USpeeleSaveGame* SaveGameInstance = Cast<USpeeleSaveGame>(UGameplayStatics::CreateSaveGameObject(USpeeleSaveGame::StaticClass()));

    if (SaveGameInstance)
    {
        // Save the player's position
        ACilveks* PlayerCharacter = Cast<ACilveks>(UGameplayStatics::GetPlayerCharacter(this, 0));
        if (PlayerCharacter)
        {
            SaveGameInstance->PlayerPosition = PlayerCharacter->GetActorLocation();

            // Save the player's inventory
            SaveGameInstance->Inventory.Empty(); // Clear the inventory to avoid duplicates
            UInventoryComponent* CharacterInventory = PlayerCharacter->PlayerInventory;
            if (CharacterInventory)
            {
                for (const auto& Item : CharacterInventory->InventoryContents)
                {
                    if (Item)
                    {
                        FInventoryItem InventoryItem;
                        InventoryItem.ItemName = Item->ID.ToString(); // Convert FName to FString
                        SaveGameInstance->Inventory.Add(InventoryItem);

                        // Debug log to show the saved inventory item
                        UE_LOG(LogTemp, Warning, TEXT("Saved item: %s"), *InventoryItem.ItemName);
                    }
                }
            }
        }

        // Save the game
        UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("SaveSlot"), 0);

        UE_LOG(LogTemp, Warning, TEXT("Game state saved!"));
        
    }
}







void USpeeleGameInstance::LoadGame()
{
    // Load the save game object
    USpeeleSaveGame* SaveGameInstance = Cast<USpeeleSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("SaveSlot"), 0));

    if (SaveGameInstance)
    {
        // Apply the player's position
        ACilveks* PlayerCharacter = Cast<ACilveks>(UGameplayStatics::GetPlayerCharacter(this, 0));
        if (PlayerCharacter)
        {


            // Debug logs
            UE_LOG(LogTemp, Warning, TEXT("Loaded player position: %s"), *SaveGameInstance->PlayerPosition.ToString());

            // Check the loaded player position
            if (SaveGameInstance->PlayerPosition.IsZero())
            {
                UE_LOG(LogTemp, Warning, TEXT("Loaded player position is (0,0,0). Returning without loading the game."));
                return;
            }


            // Set the player's position
            PlayerCharacter->SetActorLocation(SaveGameInstance->PlayerPosition);
        }

        // Apply the player's inventory
        UInventoryComponent* CharacterInventory = PlayerCharacter->PlayerInventory;
        if (CharacterInventory)
        {
            CharacterInventory->InventoryContents.Empty(); // Clear the inventory to avoid duplicates

            for (const auto& InventoryItem : SaveGameInstance->Inventory)
            {
                // Load the item data table
                UDataTable* MyItemDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/ItemData/WeaponData.WeaponData"));
                if (MyItemDataTable)
                {
                    // Retrieve row data from the data table
                    const FItemData* ItemData = MyItemDataTable->FindRow<FItemData>(FName(*InventoryItem.ItemName), TEXT(""));
                    if (ItemData)
                    {
                        // Create a new item with the loaded data
                        UItemBase* Item = NewObject<UItemBase>();
                        if (Item)
                        {
                            // Set the item properties from the row data
                            Item->ID = ItemData->ID;
                            Item->ItemType = ItemData->ItemType;
                            Item->ItemQuality = ItemData->ItemQuality;
                            Item->NumericData = ItemData->NumericData;
                            Item->TextData = ItemData->TextData;
                            Item->AssetData = ItemData->AssetData;

                            // Add the item to the inventory
                            CharacterInventory->AddNewItem(Item, 1); // Add one item
                        }
                        else
                        {
                            UE_LOG(LogTemp, Error, TEXT("Failed to create item with ID %s"), *InventoryItem.ItemName);
                        }
                    }
                    else
                    {
                        UE_LOG(LogTemp, Error, TEXT("Item data for %s not found in the data table"), *InventoryItem.ItemName);
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("Item data table not found"));
                }
            }
        }
        
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load save game!"));
    }
}

void USpeeleGameInstance::ResetGame()
{
    // Clear the save data
    UGameplayStatics::DeleteGameInSlot(TEXT("SaveSlot"), 0);

    // Reset the player's position
    ACilveks* PlayerCharacter = Cast<ACilveks>(UGameplayStatics::GetPlayerCharacter(this, 0));
    if (PlayerCharacter)
    {
        PlayerCharacter->SetActorLocation(FVector::ZeroVector);
    }

    // Clear the player's inventory
    UInventoryComponent* CharacterInventory = PlayerCharacter->PlayerInventory;
    if (CharacterInventory)
    {
        CharacterInventory->InventoryContents.Empty();
    }

    // Save the game state
    SaveGame();
}


void USpeeleGameInstance::SetPlayerPosition(const FVector& NewPosition)
{
    // Save the player position
    USpeeleSaveGame* SaveGameInstance = Cast<USpeeleSaveGame>(UGameplayStatics::CreateSaveGameObject(USpeeleSaveGame::StaticClass()));
    if (SaveGameInstance)
    {
        SaveGameInstance->PlayerPosition = NewPosition;
        UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("SaveSlot"), 0);
    }
}

void USpeeleGameInstance::SetInventory(const TArray<FInventoryItem>& NewInventory)
{
    // Save the inventory
    USpeeleSaveGame* SaveGameInstance = Cast<USpeeleSaveGame>(UGameplayStatics::CreateSaveGameObject(USpeeleSaveGame::StaticClass()));
    if (SaveGameInstance)
    {
        SaveGameInstance->Inventory = NewInventory;
        UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("SaveSlot"), 0);
    }
}

void USpeeleGameInstance::SetNPCData(const TArray<FNPCData>& NewNPCData)
{
    // Save the NPC data
    USpeeleSaveGame* SaveGameInstance = Cast<USpeeleSaveGame>(UGameplayStatics::CreateSaveGameObject(USpeeleSaveGame::StaticClass()));
    if (SaveGameInstance)
    {
        SaveGameInstance->NPCData = NewNPCData;
        UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("SaveSlot"), 0);
    }
}



