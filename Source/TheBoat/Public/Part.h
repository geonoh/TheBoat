// It's made by Geon Oh

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Part.generated.h"

UCLASS()
class THEBOAT_API APart : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
