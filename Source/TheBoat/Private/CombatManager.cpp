// It's made by Geon Oh


#include "CombatManager.h"

#include "CombatHUD.h"
#include "EngineUtils.h"
#include "PartsSpawner.h"
#include "TheBoat/TheBoat.h"

UCombatManager::UCombatManager()
{
}

void UCombatManager::OnEnterCombatWorld(const UWorld* InWorld)
{
	InitCombatWorld();
}

void UCombatManager::InitCombatWorld()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	
	World->GetTimerManager().SetTimer(
		ItemGenerateTimerHandle,
		this,
		&UCombatManager::OnItemGenerateStart,
		5.f,
		true
	);

	LoadSpawner();
}

void UCombatManager::LoadSpawner()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	for (TActorIterator<APartsSpawner> Iter(World); Iter; ++Iter)
	{
		Spawners.Add(*Iter);
	}
}

void UCombatManager::OnItemGenerateStart() const
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	ACombatHUD* CombatHUD = Cast<ACombatHUD>(World->GetFirstPlayerController()->GetHUD());
	if (!CombatHUD)
	{
		BOAT_LOG(Error, TEXT("CombatHUD did not create"));
		check(false);
	}
}
