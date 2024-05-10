// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/StartScreenWidget.h"
#include "Components/Widget.h"
#include "SpeeleSaveGame.h"
#include "Speele/SpeeleGameMode.h" // Include the header file

#include <Kismet/GameplayStatics.h>
#include <SpeeleGameInstance.h>
#include <Speele/Cilveks.h>


void UStartScreenWidget::NativeConstruct()
{
    Super::NativeConstruct();


    if (SaveGameButton)
    {
        SaveGameButton->OnClicked.AddDynamic(this, &UStartScreenWidget::OnSaveGameClicked);
    }

    if (LoadGameButton)
    {
        LoadGameButton->OnClicked.AddDynamic(this, &UStartScreenWidget::OnLoadGameClicked);
    }

    if (NewGameButton)
    {
        NewGameButton->OnClicked.AddDynamic(this, &UStartScreenWidget::OnNewGameClicked);
    }

    if (QuitGameButton)
    {
        QuitGameButton->OnClicked.AddDynamic(this, &UStartScreenWidget::OnQuitGameClicked);
    }

}

// UStartScreenWidget.cpp
void UStartScreenWidget::OnSaveGameClicked()
{
    // Call SaveGame directly from the game instance
    USpeeleGameInstance* GameInstance = Cast<USpeeleGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    if (!GameInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get GameInstance"));
        return;
    }
    
    GameInstance->SaveGame();
}







void UStartScreenWidget::OnLoadGameClicked()
{
    // Load game instance
    USpeeleGameInstance* GameInstance = Cast<USpeeleGameInstance>(GetWorld()->GetGameInstance());
    if (GameInstance)
    {
        if (!UGameplayStatics::DoesSaveGameExist(TEXT("SaveSlot"), 0))
        {
            UE_LOG(LogTemp, Warning, TEXT("Save game file does not exist!"));
            return;
        }
        UE_LOG(LogTemp, Warning, TEXT("Loading game"));
        
        // Load the game
        GameInstance->LoadGame();

        // Toggle start screen widget visibility
        ASpeeleGameMode* GameMode = Cast<ASpeeleGameMode>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->ToggleStartScreenWidget();
        }

        // Hide mouse cursor and set input mode to game only
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            PlayerController->SetInputMode(FInputModeGameOnly());
            PlayerController->bShowMouseCursor = false;
        }

    }

}








void UStartScreenWidget::OnNewGameClicked()
{
    // Load game instance
    USpeeleGameInstance* GameInstance = Cast<USpeeleGameInstance>(GetWorld()->GetGameInstance());
    if (GameInstance)
    {
        // Reset player position
        FVector NewPlayerPosition = FVector(-9337.249441f, 17850.447596f, 3672.782902f);
        GameInstance->SetPlayerPosition(NewPlayerPosition);

        // Clear inventory
        TArray<FInventoryItem> NewInventory;
        GameInstance->SetInventory(NewInventory);

        // Clear NPC data
        TArray<FNPCData> NewNPCData;
        GameInstance->SetNPCData(NewNPCData);

        // Toggle start screen widget visibility
        ASpeeleGameMode* GameMode = Cast<ASpeeleGameMode>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->ToggleStartScreenWidget();
        }

        // Hide mouse cursor and set input mode to game only
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            PlayerController->SetInputMode(FInputModeGameOnly());
            PlayerController->bShowMouseCursor = false;
        }

        // Log a message
        UE_LOG(LogTemp, Warning, TEXT("New game started!"));
    }
}









void UStartScreenWidget::OnQuitGameClicked()
{
    // Quit the game
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        PlayerController->ConsoleCommand("quit");
    }
}




