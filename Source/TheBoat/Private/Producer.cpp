// It's made by Geon Oh


#include "Producer.h"

#include "CombatManager.h"

UProducer* UProducer::Instance = nullptr;

void UProducer::InitInstance(UGameInstance* GameInstance)
{
	Instance = NewObject<UProducer>(GameInstance);
	Instance->AddToRoot();  // Skip GC
}

void UProducer::OnShuttingDown()
{
	Instance->RemoveFromRoot();	// GC On
	Instance = nullptr;
}

UProducer::UProducer(): CombatManager(nullptr)
{
}

UProducer::~UProducer()
{
	DestroyManagers();
}

void UProducer::AllocateManagers()
{
	CombatManager = NewObject<UCombatManager>(this);
}

void UProducer::DestroyManagers()
{
	CombatManager = nullptr;
}

UCombatManager& UProducer::GetCombatManager() const
{
	if (CombatManager == nullptr)
	{
		check(false);
	}
	
	return *CombatManager;
}
