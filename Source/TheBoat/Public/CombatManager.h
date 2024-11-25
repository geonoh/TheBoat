// It's made by Geon Oh

#pragma once

#include "CoreMinimal.h"
#include "../Shared/Defines.h"
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

	void OnEndPlay();

	void UpdateCharacter(int64 CharacterId, const FCombatCharacterInfo& NewInfo);

	// Gunny TODO : It should be gotten from server
	void OnEnterCombatWorld();
	void OnCollision(ACombatCharacter* Character, APart* Part);

private:
	void ClearTimer();
	void TempSetItemGenerateTimer();	// Gunny TODO : Temocode. Server will make items
	
	void SpawnSpawners(const std::vector<FCombatSpawnerInfo>& SpawnerInfos);
	void SpawnCharacters(const std::vector<FCombatCharacterInfo>& CharacterInfos);
	void OnItemGenerateTime() const;

	FTimerHandle ItemGenerateTimerHandle;

	UPROPERTY()
	TMap<int64/*SpawnerId*/, APartsSpawner*> Spawners;

	UPROPERTY()
	TMap<int64/*CharacterId*/, ACombatCharacter*> CombatCharacters;

	int64 GameTime;
};
