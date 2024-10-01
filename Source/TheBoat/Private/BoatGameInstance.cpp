// It's made by Geon Oh


#include "BoatGameInstance.h"

#include "Producer.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "TheBoat/FPacketReceiveRunnable.h"

FSocket* CreateSocket()
{
	// 소켓 서브시스템에서 소켓 생성
	FSocket* Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);
    
	// 소켓 설정 (예: 블록킹 모드, 타임아웃 설정 등)
	int32 BufferSize = 2 * 1024 * 1024; // 버퍼 크기 설정
	Socket->SetReceiveBufferSize(BufferSize, BufferSize);
	Socket->SetSendBufferSize(BufferSize, BufferSize);

	return Socket;
}

void UBoatGameInstance::Init()
{
	Super::Init();

	UProducer::InitInstance(this);
	GetProducer().AllocateManagers();

	Socket = CreateSocket();
	PacketReceiveRunnable = new FPacketReceiveRunnable(Socket);
	PacketReceiveRunnable->Start();
}

void UBoatGameInstance::Shutdown()
{
	Super::Shutdown();
	
	GetProducer().OnShuttingDown();

	if (PacketReceiveRunnable)
	{
		PacketReceiveRunnable->Stop();
		delete PacketReceiveRunnable;
		PacketReceiveRunnable = nullptr;
	}

	if (Socket)
	{
		Socket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
	}
}
