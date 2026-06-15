// Copyright Epic Games, Inc. All Rights Reserved.

#include "NumBallGameMode.h"
#include "NumBallCharacter.h"
#include "UObject/ConstructorHelpers.h"

ANumBallGameMode::ANumBallGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
