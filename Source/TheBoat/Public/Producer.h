// It's made by Geon Oh

#pragma once

#include "CoreMinimal.h"
#include "Net/Core/DirtyNetObjectTracker/GlobalDirtyNetObjectTracker.h"
#include "TheBoat/TheBoat.h"
#include "UObject/NoExportTypes.h"
#include "Producer.generated.h"

class UCombatManager;
/**
 * Producer manages Managers
 * It makes on GameInstance Init()
 */
UCLASS()
class THEBOAT_API UProducer : public UObject
{
	GENERATED_BODY()

public:
	static UProducer& Get()
	{
		if (!Instance)
		{
			check(false);
		}

		return *Instance;
	}
	
	static void InitInstance(UGameInstance* GameInstance);
	static void OnShuttingDown();

public:
	UProducer();
	virtual ~UProducer() override;

	void AllocateManagers();
	void DestroyManagers();
	
	UCombatManager& GetCombatManager() const;
	
private:
	static UProducer* Instance;
	
	UPROPERTY()
	UCombatManager* CombatManager;
};
