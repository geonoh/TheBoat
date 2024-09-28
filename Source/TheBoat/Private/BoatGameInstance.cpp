// It's made by Geon Oh


#include "BoatGameInstance.h"

#include "Producer.h"

void UBoatGameInstance::Init()
{
	Super::Init();

	UProducer::InitInstance(NewObject<UProducer>(this));
}

void UBoatGameInstance::Shutdown()
{
	Super::Shutdown();
	
	UProducer::DestroyInstance();
}
