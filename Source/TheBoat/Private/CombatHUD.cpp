// It's made by Geon Oh


#include "CombatHUD.h"

#include "CombatWidget.h"
#include "Blueprint/UserWidget.h"
#include "TheBoat/TheBoat.h"

void ACombatHUD::BeginPlay()
{
	Super::BeginPlay();

	CombatWidget = Cast<UCombatWidget>(CreateWidget(GetWorld(), CombatWidgetClass));
	if (!CombatWidget)
	{
		BOAT_LOG(Error, TEXT("LobbyWidget does not exist."));
		check(false);
		return;
	}

	CombatWidget->AddToViewport();
	CombatWidget->Set();
}
