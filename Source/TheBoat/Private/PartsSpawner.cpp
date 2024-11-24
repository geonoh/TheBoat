// It's made by Geon Oh


#include "PartsSpawner.h"

#include "Part.h"
#include "../../../Shared/Defines.h"

// Sets default values
APartsSpawner::APartsSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpawnerId = 0;
}

APart* APartsSpawner::SpawnPart() const
{
	APart* SpawnedPart = GetWorld()->SpawnActor<APart>(
		PartClass, 
		GetActorLocation(),
		GetActorRotation()
	);

	check(SpawnedPart);
	return SpawnedPart;
}

void APartsSpawner::InitSpawner(const FCombatSpawnerInfo& Info)
{
	SpawnerId = Info.SpawnerId;
}

int64 APartsSpawner::GetSpawnerId() const
{
	return SpawnerId;
}

// Called when the game starts or when spawned
void APartsSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APartsSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

