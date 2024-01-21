// Copyright Epic Games, Inc. All Rights Reserved.

#include "FracturingGameMode.h"
#include "FracturingCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFracturingGameMode::AFracturingGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
