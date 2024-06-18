// It's made by Geon Oh


#include "CombatManager.h"

#include "CombatHUD.h"
#include "EngineUtils.h"
#include "PartsSpawner.h"
#include "TheBoat/TheBoat.h"

void UCombatManager::EnterCombat()
{
	GetWorld()->GetTimerManager().SetTimer(
		ItemGenerateTimerHandle,
		this,
		&UCombatManager::OnItemGenerateStart,
		5.f,
		true
	);
}

void UCombatManager::LoadSpawner()
{
	for (TActorIterator<APartsSpawner> Iter(GetWorld()); Iter; ++Iter)
	{
		Spawners.Add(*Iter);
	}
}

void UCombatManager::OnItemGenerateStart() const
{
	ACombatHUD* CombatHUD = Cast<ACombatHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (!CombatHUD)
	{
		BOAT_LOG(Error, TEXT("CombatHUD did not create"));
		check(false);
	}
}
