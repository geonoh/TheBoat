// It's made by Geon Oh


#include "BoatGameInstance.h"

#include "Producer.h"

void UBoatGameInstance::Init()
{
	Super::Init();

	UProducer::InitInstance(this);
	GetProducer().AllocateManagers();
}

void UBoatGameInstance::Shutdown()
{
	Super::Shutdown();
	
	GetProducer().OnShuttingDown();
}
