// It's made by Geon Oh


#include "CombatManager.h"

#include "CombatHUD.h"
#include "EngineUtils.h"
#include "Part.h"
#include "PartsSpawner.h"
#include "TheBoat/CombatGameMode.h"
#include "TheBoat/TheBoat.h"

UCombatManager::UCombatManager(): GameTime(0)
{
}

UCombatManager::~UCombatManager()
{
	Spawners.Empty();
}

void UCombatManager::OnEnterCombatWorld()
{
	Spawners.Empty();
	CombatCharacters.Empty();

	// Gunny TODO : Get CombatInfo from server
	const FCombatEnterInfo TempCombatEnterInfo
	{
		0L,
		{
			FCombatSpawnerInfo(0, 1000.0, 760.0, 0.0),
			FCombatSpawnerInfo(1, 2170.0, 2290.0, 0.0),
			FCombatSpawnerInfo(2, 1020.0, 2200.0, 0.0),
		},
		{
			FCombatCharacterInfo(0, ETeamType::First, {}),
			FCombatCharacterInfo(1, ETeamType::First, {}),
			FCombatCharacterInfo(2, ETeamType::Second, {}),
			FCombatCharacterInfo(3, ETeamType::Second, {}),
		}
	};

	TempSetItemGenerateTimer();
	SpawnSpawner(TempCombatEnterInfo.SpawnerInfos);
	SpawnCharacter(TempCombatEnterInfo.CharacterInfos);
}

void UCombatManager::OnCollision(ACombatCharacter* Character, APart* Part)
{
	// Gunny TODO
}

void UCombatManager::TempSetItemGenerateTimer()
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	World->GetTimerManager().SetTimer(
		ItemGenerateTimerHandle,
		this,
		&UCombatManager::OnItemGenerateTime,
		60.f,
		true
	);
}

void UCombatManager::SpawnSpawner(const std::vector<FCombatSpawnerInfo>& SpawnerInfos)
{
	ACombatGameMode* CombatGameMode = Cast<ACombatGameMode>(GetWorld()->GetAuthGameMode());
	check(CombatGameMode);

	for (const FCombatSpawnerInfo& SpawnerInfo : SpawnerInfos)
	{
		Spawners.Add(CombatGameMode->SpawnSpawner(SpawnerInfo));
	}
}

void UCombatManager::SpawnCharacter(const std::vector<FCombatCharacterInfo>& CharacterInfos)
{
	for (const auto& Iter : CharacterInfos)
	{
		// Gunny TODO
	}
}

void UCombatManager::OnItemGenerateTime() const
{
	ACombatHUD* CombatHUD = Cast<ACombatHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (!CombatHUD)
	{
		check(false);
	}

	const int RandomNumber = rand() % Spawners.Num();
	APart* Part = Spawners[RandomNumber]->SpawnPart();

	const EPartType PartType = static_cast<EPartType>(rand() % static_cast<int32>(EPartType::Max));
	Part->OnGenerated(PartType);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.0f,
			FColor::Yellow,
			TEXT("Item generated")
		);
	}
}
