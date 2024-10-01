#include "FPacketRunnable.h"

FPacketRunnable::FPacketRunnable(): Socket(nullptr), Thread(nullptr)
{
}

FPacketRunnable::~FPacketRunnable()
{
	if (Thread)
	{
		delete Thread;
		Thread = nullptr;
	}
}

bool FPacketRunnable::Init()
{
	return FRunnable::Init();
}

uint32 FPacketRunnable::Run()
{
	while (StopTaskCounter.GetValue() == 0)
	{
		// Gunny TODO : Process packet
	}

	return 0;
}

void FPacketRunnable::Stop()
{
	StopTaskCounter.Increment();
	FRunnable::Stop();
}

void FPacketRunnable::Start()
{
	if (Thread)
	{
		return;
	}

	Thread = FRunnableThread::Create(
		this,
		TEXT("FPacketReceiverRunnable"),
		0,
		TPri_BelowNormal
	);
}
