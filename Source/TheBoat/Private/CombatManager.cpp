// It's made by Geon Oh


#include "CombatManager.h"

#include "CombatHUD.h"
#include "EngineUtils.h"
#include "Part.h"
#include "PartsSpawner.h"
#include "TheBoat/TheBoat.h"

UCombatManager::UCombatManager()
{
}

UCombatManager::~UCombatManager()
{
	Spawners.Empty();
}

void UCombatManager::OnEnterCombatWorld()
{
	InitCombatWorld();
}

void UCombatManager::OnCollision(ACombatCharacter* Character, APart* Part)
{
	// Gunny TODO
}

void UCombatManager::InitCombatWorld()
{
	StartItemGenTimer();
	LoadSpawner();
}

void UCombatManager::StartItemGenTimer()
{
	const UWorld* World = GetWorld();
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
