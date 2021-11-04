// Copyright Epic Games, Inc. All Rights Reserved.

#include "GladiatorGameGameMode.h"
#include "GladiatorGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGladiatorGameGameMode::AGladiatorGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Player/PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
