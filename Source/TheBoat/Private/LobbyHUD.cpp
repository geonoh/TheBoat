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
		check(false);
	}

	LobbyWidget->AddToViewport();
	LobbyWidget->Set();
}
