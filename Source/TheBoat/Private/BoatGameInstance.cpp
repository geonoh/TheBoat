// It's made by Geon Oh


#include "BoatGameInstance.h"

#include "Producer.h"

void UBoatGameInstance::Init()
{
	Super::Init();

	UProducer::InitInstance(this);
	GetProducer().InitManagers();
}

void UBoatGameInstance::Shutdown()
{
	Super::Shutdown();
	
	GetProducer().OnShuttingDown();
}
