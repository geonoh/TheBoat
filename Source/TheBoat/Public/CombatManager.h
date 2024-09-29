// It's made by Geon Oh

#pragma once

#include "CoreMinimal.h"
#include "TheBoat/Defines.h"
#include "CombatManager.generated.h"

struct FCombatEnterInfo;
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

	// Gunny TODO : It should be gotten from server
	void OnEnterCombatWorld();
	void OnCollision(ACombatCharacter* Character, APart* Part);
	
private:
	void TempSetItemGenerateTimer();	// Gunny TODO : Temocode. Server will make items
	
	void SpawnSpawner(const std::vector<FCombatSpawnerInfo>& SpawnerInfos);
	void SpawnCharacter(const std::vector<FCombatCharacterInfo>& CharacterInfos);
	void OnItemGenerateTime() const;

	FTimerHandle ItemGenerateTimerHandle;

	UPROPERTY()
	TArray<APartsSpawner*> Spawners;

	UPROPERTY()
	TArray<ACombatCharacter*> CombatCharacters;

	int64 GameTime;
};
