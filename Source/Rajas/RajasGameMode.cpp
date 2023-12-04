// Copyright Epic Games, Inc. All Rights Reserved.

#include "RajasGameMode.h"
#include "RajasCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARajasGameMode::ARajasGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
