// It's made by Geon Oh

#include "BoatGameInstance.h"

#include "Containers/Ticker.h"
#include "HAL/PlatformProcess.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Producer.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "TheBoat/FPacketReceiveRunnable.h"
#include "TheBoat/TheBoat.h"

FSocket* CreateSocket()
{
	ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	if (!SocketSubsystem)
	{
		return nullptr;
	}

	FSocket* NewSocket = SocketSubsystem->CreateSocket(NAME_Stream, TEXT("BoatClientSocket"), false);
	if (!NewSocket)
	{
		return nullptr;
	}

	int32 BufferSize = 2 * 1024 * 1024;
	NewSocket->SetReceiveBufferSize(BufferSize, BufferSize);
	NewSocket->SetSendBufferSize(BufferSize, BufferSize);
	NewSocket->SetNoDelay(true);
	NewSocket->SetNonBlocking(false);

	return NewSocket;
}

void UBoatGameInstance::Init()
{
	Super::Init();

	UProducer::InitInstance(this);
	GetProducer().AllocateManagers();

	Socket = CreateSocket();

	TickHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UBoatGameInstance::TickServerMessages));
}

void UBoatGameInstance::Shutdown()
{
	DisconnectFromServer();

	if (TickHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(TickHandle);
		TickHandle.Reset();
	}

	GetProducer().OnShuttingDown();

	Super::Shutdown();
}

void UBoatGameInstance::SetPassthroughMode(const bool bSet)
{
	bIsPassthroughMode = bSet;
}

bool UBoatGameInstance::IsPassthroughMode() const
{
	return bIsPassthroughMode;
}

bool UBoatGameInstance::ConnectToServer()
{
	if (bIsConnected)
	{
		return true;
	}

	if (!Socket)
	{
		Socket = CreateSocket();
	}

	if (!Socket)
	{
		UpdateStatusMessage(TEXT("Failed to create client socket"));
		return false;
	}

	ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	if (!SocketSubsystem)
	{
		UpdateStatusMessage(TEXT("Socket subsystem unavailable"));
		return false;
	}

	TSharedRef<FInternetAddr> InternetAddress = SocketSubsystem->CreateInternetAddr();
	FIPv4Address ParsedAddress;
	const bool bIsLocalhost = ServerHost.Equals(TEXT("localhost"), ESearchCase::IgnoreCase);
	if (bIsLocalhost)
	{
		ParsedAddress = FIPv4Address::InternalLoopback;
	}

	if (!bIsLocalhost && !FIPv4Address::Parse(ServerHost, ParsedAddress))
	{
		UpdateStatusMessage(FString::Printf(TEXT("Invalid server host: %s"), *ServerHost));
		return false;
	}

	InternetAddress->SetIp(ParsedAddress.Value);
	InternetAddress->SetPort(ServerPort);

	if (!Socket->Connect(*InternetAddress))
	{
		const ESocketErrors SocketError = SocketSubsystem->GetLastErrorCode();
		UpdateStatusMessage(FString::Printf(TEXT("Connect failed: %s"), SocketSubsystem->GetSocketError(SocketError)));
		return false;
	}

	PacketReceiveRunnable = new FPacketReceiveRunnable(Socket, &PendingPackets);
	PacketReceiveRunnable->Start();
	bIsConnected = true;
	UpdateStatusMessage(FString::Printf(TEXT("Connected to %s:%d"), *ServerHost, ServerPort));
	return true;
}

void UBoatGameInstance::DisconnectFromServer()
{
	bIsConnected = false;
	bIsLoggedIn = false;
	bIsInQueue = false;
	SessionId = 0;
	MatchId = 0;

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
		Socket = nullptr;
	}

	UpdateStatusMessage(TEXT("Disconnected"));
}

bool UBoatGameInstance::SendHello(const FString& InPlayerName)
{
	PlayerName = InPlayerName.IsEmpty() ? FPlatformProcess::UserName(false) : InPlayerName;
	return SendPacket(FString::Printf(TEXT("HELLO %s"), *PlayerName));
}

bool UBoatGameInstance::SendQueueJoin()
{
	if (!bIsLoggedIn)
	{
		UpdateStatusMessage(TEXT("Login is required before queueing"));
		return false;
	}

	return SendPacket(TEXT("QUEUE"));
}

bool UBoatGameInstance::SendMove(const FVector& InWorldPosition)
{
	if (!bIsConnected || MatchId == 0)
	{
		return false;
	}

	return SendPacket(FString::Printf(TEXT("MOVE %.2f %.2f %.2f"), InWorldPosition.X, InWorldPosition.Y, InWorldPosition.Z));
}

bool UBoatGameInstance::SendFire(const FVector& InOrigin, const FVector& InDirection)
{
	if (!bIsConnected || MatchId == 0)
	{
		return false;
	}

	return SendPacket(
		FString::Printf(TEXT("FIRE %.2f %.2f %.2f %.2f %.2f %.2f"),
			InOrigin.X, InOrigin.Y, InOrigin.Z,
			InDirection.X, InDirection.Y, InDirection.Z));
}

bool UBoatGameInstance::IsConnectedToBoatServer() const
{
	return bIsConnected;
}

bool UBoatGameInstance::IsLoggedInToBoatServer() const
{
	return bIsLoggedIn;
}

const FString& UBoatGameInstance::GetStatusMessage() const
{
	return StatusMessage;
}

int64 UBoatGameInstance::GetSessionId() const
{
	return SessionId;
}

int64 UBoatGameInstance::GetMatchId() const
{
	return MatchId;
}

FBoatStatusMessageChanged& UBoatGameInstance::OnStatusMessageChanged()
{
	return StatusMessageChangedEvent;
}

FBoatLoginSucceeded& UBoatGameInstance::OnLoginSucceeded()
{
	return LoginSucceededEvent;
}

FBoatMatchCreated& UBoatGameInstance::OnMatchCreated()
{
	return MatchCreatedEvent;
}

bool UBoatGameInstance::SendPacket(const FString& InPacket)
{
	if (!bIsConnected || !Socket)
	{
		UpdateStatusMessage(TEXT("Not connected to server"));
		return false;
	}

	const FString PacketWithDelimiter = InPacket + LINE_TERMINATOR;
	FTCHARToUTF8 ConvertedPacket(*PacketWithDelimiter);
	int32 BytesSent = 0;
	if (!Socket->Send(reinterpret_cast<const uint8*>(ConvertedPacket.Get()), ConvertedPacket.Length(), BytesSent))
	{
		UpdateStatusMessage(TEXT("Failed to send packet"));
		return false;
	}

	if (BytesSent != ConvertedPacket.Length())
	{
		UpdateStatusMessage(TEXT("Partial packet send detected"));
		return false;
	}

	return true;
}

bool UBoatGameInstance::TickServerMessages(float InDeltaTime)
{
	ProcessIncomingPackets();
	return true;
}

void UBoatGameInstance::ProcessIncomingPackets()
{
	FString Packet;
	while (PendingPackets.Dequeue(Packet))
	{
		ProcessPacket(Packet);
	}
}

void UBoatGameInstance::ProcessPacket(const FString& InPacket)
{
	TArray<FString> Tokens;
	InPacket.ParseIntoArrayWS(Tokens);
	if (Tokens.IsEmpty())
	{
		return;
	}

	const FString& PacketType = Tokens[0];
	if (PacketType == TEXT("WELCOME"))
	{
		HandleWelcomePacket(Tokens);
		return;
	}

	if (PacketType == TEXT("HELLO_ACK"))
	{
		HandleHelloAckPacket(Tokens);
		return;
	}

	if (PacketType == TEXT("QUEUE_ACK"))
	{
		HandleQueueAckPacket();
		return;
	}

	if (PacketType == TEXT("MATCH_CREATED"))
	{
		HandleMatchCreatedPacket(Tokens);
		return;
	}

	if (PacketType == TEXT("ERROR"))
	{
		HandleErrorPacket(Tokens);
		return;
	}

	if (PacketType == TEXT("MATCH_STATE") || PacketType == TEXT("SNAPSHOT") || PacketType == TEXT("PLAYER_MOVED") ||
		PacketType == TEXT("PLAYER_FIRED") || PacketType == TEXT("PLAYER_LEFT") || PacketType == TEXT("PONG"))
	{
		HandleSimpleStatusPacket(InPacket);
		return;
	}

	UpdateStatusMessage(FString::Printf(TEXT("Received: %s"), *InPacket));
}

void UBoatGameInstance::UpdateStatusMessage(const FString& InStatusMessage)
{
	StatusMessage = InStatusMessage;
	StatusMessageChangedEvent.Broadcast(StatusMessage);
	UE_LOG(BOAT_LOG, Log, TEXT("%s"), *StatusMessage);
}

bool UBoatGameInstance::HandleWelcomePacket(const TArray<FString>& InTokens)
{
	if (InTokens.Num() < 2)
	{
		return false;
	}

	SessionId = FCString::Atoi64(*InTokens[1]);
	UpdateStatusMessage(FString::Printf(TEXT("Session assigned: %lld"), SessionId));
	return true;
}

bool UBoatGameInstance::HandleHelloAckPacket(const TArray<FString>& InTokens)
{
	bIsLoggedIn = true;
	if (InTokens.Num() < 2)
	{
		UpdateStatusMessage(FString::Printf(TEXT("Login complete: %s"), *PlayerName));
		LoginSucceededEvent.Broadcast();
		return true;
	}

	PlayerName = InTokens[1];
	UpdateStatusMessage(FString::Printf(TEXT("Login complete: %s"), *PlayerName));
	LoginSucceededEvent.Broadcast();
	return true;
}

bool UBoatGameInstance::HandleQueueAckPacket()
{
	bIsInQueue = true;
	UpdateStatusMessage(TEXT("Matchmaking queue joined"));
	return true;
}

bool UBoatGameInstance::HandleMatchCreatedPacket(const TArray<FString>& InTokens)
{
	if (InTokens.Num() < 2)
	{
		return false;
	}

	MatchId = FCString::Atoi64(*InTokens[1]);
	bIsInQueue = false;
	UpdateStatusMessage(FString::Printf(TEXT("Match created: %lld"), MatchId));
	MatchCreatedEvent.Broadcast(MatchId);
	return true;
}

bool UBoatGameInstance::HandleErrorPacket(const TArray<FString>& InTokens)
{
	if (InTokens.Num() < 2)
	{
		UpdateStatusMessage(TEXT("Server error: Unknown server error"));
		return true;
	}

	FString ErrorMessage = InTokens[1];
	for (int32 Index = 2; Index < InTokens.Num(); ++Index)
	{
		ErrorMessage += TEXT(" ");
		ErrorMessage += InTokens[Index];
	}

	UpdateStatusMessage(FString::Printf(TEXT("Server error: %s"), *ErrorMessage));
	return true;
}

bool UBoatGameInstance::HandleSimpleStatusPacket(const FString& InStatusMessage)
{
	UpdateStatusMessage(InStatusMessage);
	return true;
}
