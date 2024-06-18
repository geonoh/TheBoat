// Copyright Epic Games, Inc. All Rights Reserved.

#include "CombatGameMode.h"

#include "CombatManager.h"
#include "ManagerBase.h"
#include "UObject/ConstructorHelpers.h"

ACombatGameMode::ACombatGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_CombatCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
}

void ACombatGameMode::BeginPlay()
{
	Super::BeginPlay();
	GetManagers().GetCombatManager().OnEnterCombatWorld(GetWorld());
}
