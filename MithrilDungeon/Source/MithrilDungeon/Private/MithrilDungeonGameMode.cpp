// Copyright Epic Games, Inc. All Rights Reserved.

#include "MithrilDungeonGameMode.h"
#include "MithrilDungeonCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMithrilDungeonGameMode::AMithrilDungeonGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
