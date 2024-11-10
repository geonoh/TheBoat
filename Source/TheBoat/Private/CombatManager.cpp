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

void UCombatManager::OnEndPlay()
{
	ClearTimer();
}

void UCombatManager::OnEnterCombatWorld()
{
	Spawners.Empty();
	CombatCharacters.Empty();

	// Gunny TODO : Get MyCharacterId, CombatInfo from server
	const int64 MyCharacterId = 0;
	const FCombatEnterInfo TempCombatEnterInfo
	{
		0L,
		{
			FCombatSpawnerInfo(0, {1000.0, 760.0, 0.0}),
			FCombatSpawnerInfo(1, {2170.0, 2290.0, 0.0}),
			FCombatSpawnerInfo(2, {1020.0, 2200.0, 0.0}),
		},
		{
			FCombatCharacterInfo(0, ETeamType::First, {100.0, 200.0, 300.0 }, {}),
			FCombatCharacterInfo(1, ETeamType::First, {200.0, 300.0, 400.0}, {}),
			FCombatCharacterInfo(2, ETeamType::Second, {300.0, 400.0, 500.0}, {}),
			FCombatCharacterInfo(3, ETeamType::Second, {400.0, 500.0, 600.0}, {}),
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

void UCombatManager::ClearTimer()
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	World->GetTimerManager().ClearTimer(ItemGenerateTimerHandle);
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
	ACombatGameMode* CombatGameMode = Cast<ACombatGameMode>(GetWorld()->GetAuthGameMode());
	check(CombatGameMode);

	for (const FCombatCharacterInfo& Iter : CharacterInfos)
	{
		CombatCharacters.Add(CombatGameMode->SpawnCombatCharacter(Iter));
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
