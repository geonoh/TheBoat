// It's made by Geon Oh

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PartsSpawner.generated.h"

enum class EPartType;
class APart;
struct FCombatSpawnerInfo;

UCLASS()
class THEBOAT_API APartsSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APartsSpawner();

	APart* SpawnPart() const;

	void InitSpawner(const FCombatSpawnerInfo& Info);
	int64 GetSpawnerId() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category="Part class")
	TSubclassOf<APart> PartClass;

private:
	UPROPERTY()
	int64 SpawnerId;
};
