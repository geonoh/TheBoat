// It's made by Geon Oh

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LoginHUD.generated.h"

class ULoginWidget;

/**
 * 
 */
UCLASS()
class THEBOAT_API ALoginHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<ULoginWidget> LoginWidgetClass;

private:
	UPROPERTY()
	ULoginWidget* LoginWidget;
};