// It's made by Geon Oh


#include "BoatGameInstance.h"

#include "Producer.h"
#include "TheBoat/FPacketRunnable.h"

void UBoatGameInstance::Init()
{
	Super::Init();

	UProducer::InitInstance(this);
	GetProducer().AllocateManagers();

	PacketRunnable = new FPacketRunnable();
	PacketRunnable->Start();
}

void UBoatGameInstance::Shutdown()
{
	Super::Shutdown();
	
	GetProducer().OnShuttingDown();

	if (PacketRunnable)
	{
		PacketRunnable->Stop();
		delete PacketRunnable;
		PacketRunnable = nullptr;	
	}
}
