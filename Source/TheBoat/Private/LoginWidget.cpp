// It's made by Geon Oh


#include "LoginWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void ULoginWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void ULoginWidget::Set()
{
	LoginText->SetText(FText::FromString(FString("What the fuck")));
}
