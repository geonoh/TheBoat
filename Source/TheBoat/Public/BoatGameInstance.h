// It's made by Geon Oh

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BoatGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class THEBOAT_API UBoatGameInstance : public UGameInstance
{
	GENERATED_BODY()

	virtual void Init() override;
};
