#include "FPacketReceiveRunnable.h"
#include "Sockets.h"

FPacketReceiveRunnable::FPacketReceiveRunnable(): Socket(nullptr), Thread(nullptr)
{
}

FPacketReceiveRunnable::FPacketReceiveRunnable(FSocket* InSocket)
	: Socket(InSocket), Thread(nullptr)
{
}

FPacketReceiveRunnable::~FPacketReceiveRunnable()
{
	if (Thread)
	{
		delete Thread;
		Thread = nullptr;
	}
}

bool FPacketReceiveRunnable::Init()
{
	return FRunnable::Init();
}

uint32 FPacketReceiveRunnable::Run()
{
	while (StopTaskCounter.GetValue() == 0)
	{
		if (Socket && Socket->Wait(ESocketWaitConditions::WaitForRead, FTimespan::FromSeconds(1.0f)))
		{
			if (uint32 Size; Socket->HasPendingData(Size))
			{
				TArray<uint8> ReceivedData;
				ReceivedData.SetNumUninitialized(FMath::Min(Size, 65507u)); // 최대 패킷 크기 설정
				int32 BytesRead = 0;
				Socket->Recv(ReceivedData.GetData(), ReceivedData.Num(), BytesRead);
				// 수신된 데이터를 처리
			}
		}
	}

	return 0;
}

void FPacketReceiveRunnable::Stop()
{
	StopTaskCounter.Increment();
	FRunnable::Stop();
}

void FPacketReceiveRunnable::Start()
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
