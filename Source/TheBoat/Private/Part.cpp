// It's made by Geon Oh


#include "Part.h"

// Sets default values
APart::APart()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APart::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

