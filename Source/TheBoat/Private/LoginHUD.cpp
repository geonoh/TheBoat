// It's made by Geon Oh


#include "LoginHUD.h"

#include "Blueprint/UserWidget.h"
#include "LoginWidget.h"
#include "TheBoat/TheBoat.h"

void ALoginHUD::BeginPlay()
{
	Super::BeginPlay();

	LoginWidget = Cast<ULoginWidget>(CreateWidget(GetWorld(), LoginWidgetClass));
	if (!LoginWidget)
	{
		BOAT_LOG(Error, TEXT("LoginWidget does not exist"));
		check(false);
		return;
	}
	LoginWidget->AddToViewport();
	LoginWidget->Set();
}
