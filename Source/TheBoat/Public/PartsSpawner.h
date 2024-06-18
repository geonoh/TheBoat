// It's made by Geon Oh

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PartsSpawner.generated.h"

UCLASS()
class THEBOAT_API APartsSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APartsSpawner();

	void SpawnPart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
