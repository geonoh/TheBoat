// It's made by Geon Oh

#pragma once

#include "CoreMinimal.h"
#include "ManagerBase.h"
#include "CombatManager.generated.h"

class APartsSpawner;
/**
 * 
 */
UCLASS()
class THEBOAT_API UCombatManager : public UManagerBase
{
	GENERATED_BODY()

public:
	void EnterCombat();

private:
	void LoadSpawner();
	void OnItemGenerateStart() const;

	FTimerHandle ItemGenerateTimerHandle;
	TArray<APartsSpawner*> Spawners;
};
