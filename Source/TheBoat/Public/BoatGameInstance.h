// It's made by Geon Oh

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "HAL/ThreadSafeBool.h"
#include "Containers/Queue.h"
#include "Containers/Ticker.h"
#include "BoatGameInstance.generated.h"

class FPacketReceiveRunnable;
class FSocket;

DECLARE_MULTICAST_DELEGATE_OneParam(FBoatStatusMessageChanged, const FString&);
DECLARE_MULTICAST_DELEGATE(FBoatLoginSucceeded);
DECLARE_MULTICAST_DELEGATE_OneParam(FBoatMatchCreated, int64);

UCLASS(Config=Game)
class THEBOAT_API UBoatGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void Shutdown() override;

	void SetPassthroughMode(bool bSet);
	bool IsPassthroughMode() const;

	bool ConnectToServer();
	void DisconnectFromServer();

	bool SendHello(const FString& InPlayerName);
	bool SendQueueJoin();
	bool SendMove(const FVector& InWorldPosition);
	bool SendFire(const FVector& InOrigin, const FVector& InDirection);

	bool IsConnectedToBoatServer() const;
	bool IsLoggedInToBoatServer() const;
	const FString& GetStatusMessage() const;
	int64 GetSessionId() const;
	int64 GetMatchId() const;

	FBoatStatusMessageChanged& OnStatusMessageChanged();
	FBoatLoginSucceeded& OnLoginSucceeded();
	FBoatMatchCreated& OnMatchCreated();

private:
	static constexpr int32 DefaultServerPort = 5000;

	bool SendPacket(const FString& InPacket);
	bool TickServerMessages(float InDeltaTime);
	void ProcessIncomingPackets();
	void ProcessPacket(const FString& InPacket);
	void UpdateStatusMessage(const FString& InStatusMessage);

	bool HandleWelcomePacket(const TArray<FString>& InTokens);
	bool HandleHelloAckPacket(const TArray<FString>& InTokens);
	bool HandleQueueAckPacket();
	bool HandleMatchCreatedPacket(const TArray<FString>& InTokens);
	bool HandleErrorPacket(const TArray<FString>& InTokens);
	bool HandleSimpleStatusPacket(const FString& InStatusMessage);

	UPROPERTY(Config)
	FString ServerHost = TEXT("127.0.0.1");

	UPROPERTY(Config)
	int32 ServerPort = DefaultServerPort;

	FSocket* Socket = nullptr;
	FPacketReceiveRunnable* PacketReceiveRunnable = nullptr;
	TQueue<FString, EQueueMode::Mpsc> PendingPackets{};
	FTSTicker::FDelegateHandle TickHandle{};
	FThreadSafeBool bIsConnected = false;
	bool bIsLoggedIn = false;
	bool bIsInQueue = false;
	bool bIsPassthroughMode = false;
	int64 SessionId = 0;
	int64 MatchId = 0;
	FString PlayerName{};
	FString StatusMessage = TEXT("Disconnected");
	FBoatStatusMessageChanged StatusMessageChangedEvent{};
	FBoatLoginSucceeded LoginSucceededEvent{};
	FBoatMatchCreated MatchCreatedEvent{};
};

FSocket* CreateSocket();
