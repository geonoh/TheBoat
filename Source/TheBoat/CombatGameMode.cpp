// Copyright Epic Games, Inc. All Rights Reserved.

#include "CombatGameMode.h"

#include "CombatManager.h"
#include "TheBoat.h"
#include "UObject/ConstructorHelpers.h"

ACombatGameMode::ACombatGameMode()
	: Super(), CombatManager(nullptr)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_CombatCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
}

void ACombatGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	CombatManager = NewObject<UCombatManager>(GetWorld());
	if (!CombatManager)
	{
		BOAT_LOG(Error, TEXT("CombatManager create failed"));
		return;
	}

	CombatManager->OnEnterCombatWorld(GetWorld());
}

void ACombatGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CombatManager = nullptr;
	Super::EndPlay(EndPlayReason);
}
