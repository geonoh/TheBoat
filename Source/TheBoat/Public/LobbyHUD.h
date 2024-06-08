// It's made by Geon Oh

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LobbyHUD.generated.h"

class ULobbyWidget;
/**
 * 
 */
UCLASS()
class THEBOAT_API ALobbyHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<ULobbyWidget> LobbyWidgetClass;

private:
	UPROPERTY()
	ULobbyWidget* LobbyWidget;
};
