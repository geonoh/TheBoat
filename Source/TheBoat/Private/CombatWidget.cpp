// It's made by Geon Oh


#include "CombatWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UCombatWidget::Set()
{
	const FString LeftTimeStringFormat(TEXT("남은시간 : %d"));
	const FText LeftTimeText = FText::FromString(LeftTimeStringFormat);
	LeftTime->SetText(LeftTimeText);
	AmmoProgressBar->SetPercent(50.f);
}

void UCombatWidget::ShowItemGenerateSign()
{
	PlayAnimation(ItemGenAnim);
}

void UCombatWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCombatWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Gunny TODO : Calculate left time
}
