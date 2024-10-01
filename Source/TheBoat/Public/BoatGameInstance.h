// It's made by Geon Oh

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BoatGameInstance.generated.h"

class FPacketReceiveRunnable;
/**
 * 
 */
UCLASS()
class THEBOAT_API UBoatGameInstance : public UGameInstance
{
	GENERATED_BODY()

	virtual void Init() override;
	virtual void Shutdown() override;

private:
	FSocket* Socket;
	FPacketReceiveRunnable* PacketReceiveRunnable;
};

FSocket* CreateSocket();