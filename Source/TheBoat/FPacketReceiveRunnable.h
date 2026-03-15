#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "Containers/Queue.h"

class FSocket;

class FPacketReceiveRunnable : public FRunnable
{
public:
	FPacketReceiveRunnable();
	explicit FPacketReceiveRunnable(FSocket* InSocket, TQueue<FString, EQueueMode::Mpsc>* InPendingPackets);
	virtual ~FPacketReceiveRunnable() override;

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;

	void Start();

private:
	FSocket* Socket;
	TQueue<FString, EQueueMode::Mpsc>* PendingPackets;
	FThreadSafeCounter StopTaskCounter;
	FRunnableThread* Thread;
	FString ReceiveBuffer;
};
