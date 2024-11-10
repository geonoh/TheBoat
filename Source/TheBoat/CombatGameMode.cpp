// Copyright Epic Games, Inc. All Rights Reserved.

#include "CombatGameMode.h"

#include "CombatCharacter.h"
#include "CombatManager.h"
#include "CombatPlayerController.h"
#include "PartsSpawner.h"
#include "Producer.h"
#include "TheBoat.h"
#include "UObject/ConstructorHelpers.h"

ACombatGameMode::ACombatGameMode()
	: Super()
{
}

APartsSpawner* ACombatGameMode::SpawnSpawner(const FCombatSpawnerInfo& Info)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	APartsSpawner* RetValue =
		GetWorld()->SpawnActor<APartsSpawner>(
			PartsSpawnerClass,
			FVector(Info.Pos.X, Info.Pos.Y, Info.Pos.Z),
			FRotator::ZeroRotator,
			SpawnParams
		);
	return RetValue;
}

ACombatCharacter* ACombatGameMode::SpawnCombatCharacter(const FCombatCharacterInfo& Info)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	ACombatCharacter* RetValue =
		GetWorld()->SpawnActor<ACombatCharacter>(
			CombatCharacterClass,
			FVector(Info.Pos.X, Info.Pos.Y, Info.Pos.Z),
			FRotator::ZeroRotator,
			SpawnParams
		);
	return RetValue;
}

void ACombatGameMode::BeginPlay()
{
	Super::BeginPlay();
	GetProducer().GetCombatManager().OnEnterCombatWorld(/* Gunny TODO : Get Combat data from  server */);
}

void ACombatGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetProducer().GetCombatManager().OnEndPlay();
	Super::EndPlay(EndPlayReason);
}
