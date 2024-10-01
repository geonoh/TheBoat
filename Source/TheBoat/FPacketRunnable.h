#pragma once

class FPacketRunnable : public FRunnable
{
public:
	FPacketRunnable();
	virtual ~FPacketRunnable() override;

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	
	void Start();
	
private:
	FSocket* Socket;
	FThreadSafeCounter StopTaskCounter;
	FRunnableThread* Thread;
};
