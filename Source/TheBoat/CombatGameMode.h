// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CombatGameMode.generated.h"

UCLASS(minimalapi)
class ACombatGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACombatGameMode();

protected:
	virtual void BeginPlay() override;

private:
	void OnItemGenerateStart();

	FTimerHandle ItemGenerateTimerHandle;
};
