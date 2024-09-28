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
			BOAT_LOG(Error, TEXT("UProducer : Use before init"));
		}

		return *Instance;
	}
	
	static void InitInstance(UGameInstance* GameInstance);

public:
	UProducer();
	virtual ~UProducer() override;
	
	void OnShuttingDown();

	void InitManagers();
	void DestroyManagers();
	
	UCombatManager& GetCombatManager() const;
	
private:
	static UProducer* Instance;
	
	UPROPERTY()
	UCombatManager* CombatManager;
};
