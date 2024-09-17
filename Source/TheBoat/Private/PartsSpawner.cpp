// It's made by Geon Oh


#include "PartsSpawner.h"

#include "Part.h"

// Sets default values
APartsSpawner::APartsSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void APartsSpawner::SpawnPart()
{
	APart* SpawnedPart = GetWorld()->SpawnActor<APart>(
		PartClass, 
		GetActorLocation(),
		GetActorRotation()
	);
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

