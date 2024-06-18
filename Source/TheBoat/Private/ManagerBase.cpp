// It's made by Geon Oh


#include "ManagerBase.h"

#include "CombatManager.h"

UManagerBase* UManagerBase::Instance = nullptr;

UManagerBase::UManagerBase()
{
	CombatManager = NewObject<UCombatManager>();
}

UManagerBase::~UManagerBase()
{
	CombatManager = nullptr;
}

UCombatManager& UManagerBase::GetCombatManager() const
{
	return *CombatManager;
}

UManagerBase& GetManagers()
{
	return UManagerBase::Get();
}
