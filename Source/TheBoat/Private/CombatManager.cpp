// It's made by Geon Oh


#include "CombatManager.h"

#include "CombatHUD.h"
#include "EngineUtils.h"
#include "PartsSpawner.h"
#include "TheBoat/TheBoat.h"

UCombatManager::UCombatManager(): World(nullptr)
{
}

void UCombatManager::OnEnterCombatWorld(const UWorld* InWorld)
{
	if (!InWorld)
	{
		return;
	}

	World = InWorld;

	InWorld->GetTimerManager().SetTimer(
		ItemGenerateTimerHandle,
		this,
		&UCombatManager::OnItemGenerateStart,
		5.f,
		true
	);
}

void UCombatManager::LoadSpawner()
{
	if (!World)
	{
		BOAT_LOG(Error, TEXT("OnEnterCombatWorld First"));
		return;
	}

	for (TActorIterator<APartsSpawner> Iter(World); Iter; ++Iter)
	{
		Spawners.Add(*Iter);
	}
}

void UCombatManager::OnItemGenerateStart() const
{
	if (!World)
	{
		BOAT_LOG(Error, TEXT("OnEnterCombatWorld First"));
		return;
	}

	ACombatHUD* CombatHUD = Cast<ACombatHUD>(World->GetFirstPlayerController()->GetHUD());
	if (!CombatHUD)
	{
		BOAT_LOG(Error, TEXT("CombatHUD did not create"));
		check(false);
	}
}
