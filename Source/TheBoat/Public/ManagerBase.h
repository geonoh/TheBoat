// It's made by Geon Oh

#pragma once

#include "CoreMinimal.h"
#include "ManagerBase.generated.h"

class UCombatManager;
/**
 * 
 */
UCLASS()
class THEBOAT_API UManagerBase : public UObject
{
	GENERATED_BODY()
	
public:
	UManagerBase();
	virtual ~UManagerBase() override;

	static UManagerBase& Get()
	{
		if (!Instance)
		{
			Instance = NewObject<UManagerBase>();
		}

		return *Instance;
	}

	UCombatManager& GetCombatManager() const;

private:
	UPROPERTY()
	UCombatManager* CombatManager;

	static UManagerBase* Instance;
};

extern UManagerBase& GetManagers();