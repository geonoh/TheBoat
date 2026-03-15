#include "FPacketReceiveRunnable.h"

#include "Containers/StringConv.h"
#include "HAL/PlatformProcess.h"
#include "Sockets.h"

FPacketReceiveRunnable::FPacketReceiveRunnable()
	: Socket(nullptr), PendingPackets(nullptr), Thread(nullptr)
{
}

FPacketReceiveRunnable::FPacketReceiveRunnable(FSocket* InSocket, TQueue<FString, EQueueMode::Mpsc>* InPendingPackets)
	: Socket(InSocket), PendingPackets(InPendingPackets), Thread(nullptr)
{
}

FPacketReceiveRunnable::~FPacketReceiveRunnable()
{
	if (Thread)
	{
		Thread->WaitForCompletion();
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
				ReceivedData.SetNumUninitialized(FMath::Min(Size, 65507u));
				int32 BytesRead = 0;
				if (!Socket->Recv(ReceivedData.GetData(), ReceivedData.Num(), BytesRead) || BytesRead <= 0)
				{
					continue;
				}

				FUTF8ToTCHAR Converted(reinterpret_cast<const ANSICHAR*>(ReceivedData.GetData()), BytesRead);
				ReceiveBuffer.AppendChars(Converted.Get(), Converted.Length());

				int32 NewLineIndex = INDEX_NONE;
				while (ReceiveBuffer.FindChar(TEXT('\n'), NewLineIndex))
				{
					FString Packet = ReceiveBuffer.Left(NewLineIndex);
					Packet.RemoveFromEnd(TEXT("\r"));
					ReceiveBuffer.RightChopInline(NewLineIndex + 1, EAllowShrinking::No);

					if (!Packet.IsEmpty() && PendingPackets)
					{
						PendingPackets->Enqueue(Packet);
					}
				}
			}
		}
		else
		{
			FPlatformProcess::Sleep(0.01f);
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
