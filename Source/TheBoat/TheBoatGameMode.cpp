// Copyright Epic Games, Inc. All Rights Reserved.

#include "TheBoatGameMode.h"
#include "TheBoatCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATheBoatGameMode::ATheBoatGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
