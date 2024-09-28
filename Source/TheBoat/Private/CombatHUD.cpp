// It's made by Geon Oh


#include "CombatHUD.h"

#include "CombatWidget.h"
#include "Blueprint/UserWidget.h"
#include "TheBoat/TheBoat.h"

void ACombatHUD::OnItemGenerateStart()
{
	if (!CombatWidget)
	{
		check(false);
		return;
	}

	CombatWidget->ShowItemGenerateSign();
}

void ACombatHUD::BeginPlay()
{
	Super::BeginPlay();

	CombatWidget = Cast<UCombatWidget>(CreateWidget(GetWorld(), CombatWidgetClass));
	if (!CombatWidget)
	{
		check(false);
	}

	CombatWidget->AddToViewport();
	CombatWidget->Set();
}
