// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpeeleGameMode.h"
#include "SpeeleCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASpeeleGameMode::ASpeeleGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
