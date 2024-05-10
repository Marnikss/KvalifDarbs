

#include "UserInterface/TradingPanel.h"
#include "Speele/Cilveks.h"
#include "World/Pickup.h"
#include "Components/InventoryComponent.h"
#include "Components/Widget.h"

void UTradingPanel::NativeConstruct()
{
    Super::NativeConstruct();

    if (Button1)
    {
        Button1->OnClicked.AddDynamic(this, &UTradingPanel::OnButton1Clicked);
    }

    if (Button2)
    {
        Button2->OnClicked.AddDynamic(this, &UTradingPanel::OnButton2Clicked);
    }

    if (Button3)
    {
        Button3->OnClicked.AddDynamic(this, &UTradingPanel::OnButton3Clicked);
    }

    if (Button4)
    {
        Button4->OnClicked.AddDynamic(this, &UTradingPanel::OnButton4Clicked);
    }


    PlayerCharacter = Cast<ACilveks>(GetOwningPlayerPawn());

    if (PlayerCharacter)
    {
        // Get the inventory component from the player character
        PlayerInventory = PlayerCharacter->PlayerInventory;
        if (!PlayerInventory)
        {
            UE_LOG(LogTemp, Error, TEXT("Player inventory component is null!"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Player inventory component is valid!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Player character is null!"));
    }
}

void UTradingPanel::ShowPanel()
{
    if (IsValid(PanelWidget))
    {
        PanelWidget->SetVisibility(ESlateVisibility::Visible);
        UE_LOG(LogTemp, Warning, TEXT("Panel shown!"));
        bPanelVisible = true;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("PanelWidget is not valid!"));
    }
}

void UTradingPanel::HidePanel()
{
    if (IsValid(PanelWidget))
    {
        PanelWidget->SetVisibility(ESlateVisibility::Hidden);
        UE_LOG(LogTemp, Warning, TEXT("Panel hidden!"));
        bPanelVisible = false;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("PanelWidget is not valid!"));
    }
}

void UTradingPanel::TogglePanelVisibility()
{
    if (GetVisibility() == ESlateVisibility::Visible)
    {
        HidePanel();
    }
    else
    {
        ShowPanel();
    }
}


void UTradingPanel::OnButton1Clicked()
{
    UE_LOG(LogTemp, Warning, TEXT("Button 1 clicked!"));

    // Check if the PlayerCharacter is valid
    if (PlayerCharacter)
    {
        // Get the inventory component from the PlayerCharacter
        UInventoryComponent* CharacterInventory = PlayerCharacter->PlayerInventory;
        if (CharacterInventory)
        {
            int32 Item5Count = 0;
            // Count the number of items with ID 5
            for (const auto& Item : CharacterInventory->InventoryContents)
            {
                if (Item && Item->ID == "5")
                {
                    Item5Count++;
                }
            }

            if (Item5Count >= 5)
            {
                UE_LOG(LogTemp, Warning, TEXT("Inventory contains at least 5 items with ID 5. Proceeding with the trade..."));

                // Remove 5 items with ID 5 from the inventory
                for (int32 i = 0; i < 5; ++i)
                {
                    CharacterInventory->RemoveItemByID(5);
                }

                // Load the item data table
                UDataTable* MyItemDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/ItemData/WeaponData.WeaponData'"));

                if (MyItemDataTable)
                {
                    // Retrieve row data from the data table
                    const FItemData* Item6Data = MyItemDataTable->FindRow<FItemData>(FName("6"), TEXT(""));

                    if (Item6Data)
                    {
                        // Create a new item with ID 6
                        UItemBase* Item6 = NewObject<UItemBase>();
                        if (Item6)
                        {
                            // Set the item properties from the row data
                            Item6->ID = Item6Data->ID;
                            Item6->ItemType = Item6Data->ItemType;
                            Item6->ItemQuality = Item6Data->ItemQuality;
                            Item6->NumericData = Item6Data->NumericData;
                            Item6->TextData = Item6Data->TextData;
                            Item6->AssetData = Item6Data->AssetData;

                            // Add the item to the inventory
                            CharacterInventory->AddNewItem(Item6, 1); // Add one item with ID 6
                        }
                        else
                        {
                            UE_LOG(LogTemp, Error, TEXT("Failed to create item with ID 6"));
                        }
                    }
                    else
                    {
                        UE_LOG(LogTemp, Error, TEXT("Item with ID 6 not found in the data table"));
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("Item data table not found"));
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Inventory does not contain at least 5 items with ID 5"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Player inventory component is null!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Player character is null!"));
    }
}


















void UTradingPanel::OnButton2Clicked()
{
    UE_LOG(LogTemp, Warning, TEXT("Button 2 clicked!"));

    // Check if the PlayerCharacter is valid
    if (PlayerCharacter)
    {
        // Get the inventory component from the PlayerCharacter
        UInventoryComponent* CharacterInventory = PlayerCharacter->PlayerInventory;
        if (CharacterInventory)
        {
            if (CharacterInventory->HasItemWithID(6))
            {
                UE_LOG(LogTemp, Warning, TEXT("Inventory contains an item with ID 5. Removing it..."));

                // Remove item with ID 5 from the inventory
                CharacterInventory->RemoveItemByID(6);

                // Load the item data table
                UDataTable* MyItemDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/ItemData/WeaponData.WeaponData'"));

                if (MyItemDataTable)
                {
                    // Retrieve row data from the data table
                    const FItemData* Item6Data = MyItemDataTable->FindRow<FItemData>(FName("4"), TEXT(""));

                    if (Item6Data)
                    {
                        // Create a new item with ID 6
                        UItemBase* Item6 = NewObject<UItemBase>();
                        if (Item6)
                        {
                            // Set the item properties from the row data
                            Item6->ID = Item6Data->ID;
                            Item6->ItemType = Item6Data->ItemType;
                            Item6->ItemQuality = Item6Data->ItemQuality;
                            Item6->NumericData = Item6Data->NumericData;
                            Item6->TextData = Item6Data->TextData;
                            Item6->AssetData = Item6Data->AssetData;

                            // Add the item to the inventory
                            CharacterInventory->AddNewItem(Item6, 1); // Add one item with ID 6
                        }
                        else
                        {
                            UE_LOG(LogTemp, Error, TEXT("Failed to create item with ID 6"));
                        }
                    }
                    else
                    {
                        UE_LOG(LogTemp, Error, TEXT("Item with ID 6 not found in the data table"));
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("Item data table not found"));
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Inventory does not contain an item with ID 5"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Player inventory component is null!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Player character is null!"));
    }
}

void UTradingPanel::OnButton3Clicked()
{
    // Izvada brīdinājumu, ka tika noklikšķināta 3. poga
    UE_LOG(LogTemp, Warning, TEXT("Button 3 clicked!"));

    // Pārbauda, vai PlayerCharacter ir derīgs
    if (PlayerCharacter)
    {
        // Iegūst inventory komponentu no PlayerCharacter
        UInventoryComponent* CharacterInventory = PlayerCharacter->PlayerInventory;
        if (CharacterInventory)
        {
            // Definē mainīgo, lai uzglabātu 4. ID elementu skaitu
            int32 Item4Count = 0;
            // Saskaita elementus ar ID 4
            for (const auto& Item : CharacterInventory->InventoryContents)
            {
                if (Item && Item->ID == "4")
                {
                    Item4Count++;
                }
            }
            // Pārbauda, vai ir vismaz viens elements ar ID 4
            if (Item4Count >= 1)
            {
                // Izvada apstiprinājumu, ka inventory satur vismaz vienu elementu ar ID 4
                UE_LOG(LogTemp, Warning, TEXT("Inventory contains at least 1 item with ID 4. Proceeding with the trade..."));

                // Noņem 1 elementus ar ID 4 no inventory
                for (int32 i = 0; i < 1; ++i)
                {
                    CharacterInventory->RemoveItemByID(4);
                }

                // iegūst priekšmetu datu tabulas atsauci
                UDataTable* MyItemDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/ItemData/WeaponData.WeaponData'"));

                // ja datu tabula pastāv
                if (MyItemDataTable)
                {
                    // Iegūst rindas datus no datu tabulas
                    const FItemData* Item3Data = MyItemDataTable->FindRow<FItemData>(FName("3"), TEXT(""));

                    if (Item3Data)
                    {
                        // Izveido jaunu elementu ar ID 3
                        UItemBase* Item3 = NewObject<UItemBase>();
                        if (Item3)
                        {
                            // Iestata elementa īpašības no rindas datiem
                            Item3->ID = Item3Data->ID;
                            Item3->ItemType = Item3Data->ItemType;
                            Item3->ItemQuality = Item3Data->ItemQuality;
                            Item3->NumericData = Item3Data->NumericData;
                            Item3->TextData = Item3Data->TextData;
                            Item3->AssetData = Item3Data->AssetData;

                            // Pievieno elementu inventory
                            CharacterInventory->AddNewItem(Item3, 1); // pievieno vienu priekšmetu ar ID 3
                        }
                        else
                        {
                            // Izvada kļūdu, ja neizdevās izveidot elementu ar ID 3
                            UE_LOG(LogTemp, Error, TEXT("Failed to create item with ID 3"));
                        }
                    }
                    else
                    {
                        // Izvada kļūdu, ja elementu ar ID 3 neizdevās atrast datu tabulā
                        UE_LOG(LogTemp, Error, TEXT("Item with ID 3 not found in the data table"));
                    }
                }
                else
                {
                    // Izvada kļūdu, ja elementu datu tabula nav atrasta.
                    UE_LOG(LogTemp, Error, TEXT("Item data table not found"));
                }
            }
            else
            {
                // Izvada brīdinājumu, ja inventārs nesatur vismaz 1 elementus ar ID 3
                UE_LOG(LogTemp, Warning, TEXT("Inventory does not contain at least 1 items with ID 3"));
            }
        }
        else
        {
            // Izvada kļūdu, ja uz spēlētāja inventory komponentu nav atsauces.
            UE_LOG(LogTemp, Error, TEXT("Player inventory component is null!"));
        }
    }
    else
    {
        // Izvada kļūdu, ja uz spēlētāju nav atsauces
        UE_LOG(LogTemp, Error, TEXT("Player character is null!"));
    }
}

void UTradingPanel::OnButton4Clicked()
{
    UE_LOG(LogTemp, Warning, TEXT("Button 4 clicked!"));

    // Check if the PlayerCharacter is valid
    if (PlayerCharacter)
    {
        // Get the inventory component from the PlayerCharacter
        UInventoryComponent* CharacterInventory = PlayerCharacter->PlayerInventory;
        if (CharacterInventory)
        {
            int32 Item3Count = 0;
            // Count the number of items with ID 5
            for (const auto& Item : CharacterInventory->InventoryContents)
            {
                if (Item && Item->ID == "3")
                {
                    Item3Count++;
                }
            }

            if (Item3Count >= 3)
            {
                UE_LOG(LogTemp, Warning, TEXT("Inventory contains at least 3 items with ID 3. Proceeding with the trade..."));

                // Remove 5 items with ID 5 from the inventory
                for (int32 i = 0; i < 3; ++i)
                {
                    CharacterInventory->RemoveItemByID(3);
                }

                // Load the item data table
                UDataTable* MyItemDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/ItemData/WeaponData.WeaponData'"));

                if (MyItemDataTable)
                {
                    // Retrieve row data from the data table
                    const FItemData* Item1Data = MyItemDataTable->FindRow<FItemData>(FName("1"), TEXT(""));

                    if (Item1Data)
                    {
                        // Create a new item with ID 6
                        UItemBase* Item1 = NewObject<UItemBase>();
                        if (Item1)
                        {
                            // Set the item properties from the row data
                            Item1->ID = Item1Data->ID;
                            Item1->ItemType = Item1Data->ItemType;
                            Item1->ItemQuality = Item1Data->ItemQuality;
                            Item1->NumericData = Item1Data->NumericData;
                            Item1->TextData = Item1Data->TextData;
                            Item1->AssetData = Item1Data->AssetData;

                            // Add the item to the inventory
                            CharacterInventory->AddNewItem(Item1, 1); // Add one item with ID 6
                        }
                        else
                        {
                            UE_LOG(LogTemp, Error, TEXT("Failed to create item with ID 6"));
                        }
                    }
                    else
                    {
                        UE_LOG(LogTemp, Error, TEXT("Item with ID 6 not found in the data table"));
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("Item data table not found"));
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Inventory does not contain at least 5 items with ID 5"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Player inventory component is null!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Player character is null!"));
    }
}

