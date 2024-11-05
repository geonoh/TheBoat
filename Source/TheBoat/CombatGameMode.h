// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "../../Shared/Defines.h"
#include "GameFramework/GameModeBase.h"
#include "CombatGameMode.generated.h"

class ACombatCharacter;
class UCombatManager;
class APartsSpawner;

UCLASS(minimalapi)
class ACombatGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACombatGameMode();

	APartsSpawner* SpawnSpawner(const FCombatSpawnerInfo& Info);
	ACombatCharacter* SpawnCombatCharacter(const FCombatCharacterInfo& Info);

private:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APartsSpawner> PartsSpawnerClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACombatCharacter> CombatCharacterClass;
};
