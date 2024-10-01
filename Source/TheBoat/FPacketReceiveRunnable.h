#pragma once

class FPacketReceiveRunnable : public FRunnable
{
public:
	FPacketReceiveRunnable();
	explicit FPacketReceiveRunnable(FSocket* InSocket);
	virtual ~FPacketReceiveRunnable() override;

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	
	void Start();
	
private:
	FSocket* Socket;
	FThreadSafeCounter StopTaskCounter;
	FRunnableThread* Thread;
};
