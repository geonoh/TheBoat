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
class THEBOAT_API UCombatManager : public UObject
{
	GENERATED_BODY()

public:
	UCombatManager();
	void OnEnterCombatWorld(const UWorld* InWorld);

private:
	void LoadSpawner();
	void OnItemGenerateStart() const;

	FTimerHandle ItemGenerateTimerHandle;
	TArray<APartsSpawner*> Spawners;

	UPROPERTY()
	const UWorld* World;
};
