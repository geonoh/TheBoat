// It's made by Geon Oh


#include "LobbyHUD.h"

#include "LobbyWidget.h"
#include "Blueprint/UserWidget.h"
#include "TheBoat/TheBoat.h"

void ALobbyHUD::BeginPlay()
{
	Super::BeginPlay();

	// Create LobbyWidget
	LobbyWidget = Cast<ULobbyWidget>(CreateWidget(GetWorld(), LobbyWidgetClass));
	if (!LobbyWidget)
	{
		BOAT_LOG(Error, TEXT("LobbyWidget does not exist."));
		check(false);
		return;
	}

	LobbyWidget->AddToViewport();
	LobbyWidget->Set();
}
