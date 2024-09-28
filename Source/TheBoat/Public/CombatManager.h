// It's made by Geon Oh

#pragma once

#include "CoreMinimal.h"
#include "CombatManager.generated.h"

class APart;
class ACombatCharacter;
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
	virtual ~UCombatManager() override;
	void OnEnterCombatWorld();

// Gunny TODO : Network
public:
	void OnCollision(ACombatCharacter* Character, APart* Part);
	
private:
	void InitCombatWorld();
	void StartItemGenTimer();
	void LoadSpawner();
	void OnItemGenerateStart() const;

	FTimerHandle ItemGenerateTimerHandle;

	UPROPERTY()
	TArray<APartsSpawner*> Spawners;
};
