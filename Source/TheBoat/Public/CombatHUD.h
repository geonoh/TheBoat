// It's made by Geon Oh

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CombatHUD.generated.h"

class UCombatWidget;
/**
 * 
 */
UCLASS()
class THEBOAT_API ACombatHUD : public AHUD
{
	GENERATED_BODY()

public:
	void OnItemGenerateStart();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UCombatWidget> CombatWidgetClass;

private:
	UPROPERTY()
	UCombatWidget* CombatWidget;
};
