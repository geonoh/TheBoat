// It's made by Geon Oh


#include "Part.h"

#include "CombatManager.h"
#include "Producer.h"
#include "TheBoat/CombatCharacter.h"
#include "TheBoat/TheBoat.h"

// Sets default values
APart::APart()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void APart::OnGenerated(const EPartType InPartType)
{
	PartType = InPartType;
}

// Called when the game starts or when spawned
void APart::BeginPlay()
{
	Super::BeginPlay();
}

void APart::OnCollision(AActor* CollideActor)
{
	ACombatCharacter* Character = Cast<ACombatCharacter>(CollideActor);
	if (!Character)
	{
		return;
	}

	GetProducer().GetCombatManager().OnCollision(Character, this);
}

// Called every frame
void APart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

