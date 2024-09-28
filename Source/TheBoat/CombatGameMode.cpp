// Copyright Epic Games, Inc. All Rights Reserved.

#include "CombatGameMode.h"

#include "CombatManager.h"
#include "Producer.h"
#include "TheBoat.h"
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
	GetProducer().GetCombatManager().OnEnterCombatWorld();
}

void ACombatGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
