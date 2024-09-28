// It's made by Geon Oh

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Part.generated.h"

enum class EPartType;

UCLASS()
class THEBOAT_API APart : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APart();

	EPartType GetPartType() const { return PartType; }
	void OnGenerated(EPartType InPartType);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category="Part")
	void OnCollision(AActor* CollideActor);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	EPartType PartType;
};
