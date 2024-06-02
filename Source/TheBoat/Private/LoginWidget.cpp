// It's made by Geon Oh


#include "LoginWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void ULoginWidget::NativeConstruct()
{
	Super::NativeConstruct();
	LoginButton->OnClicked.AddUniqueDynamic(this, &ULoginWidget::OnLoginButtonClicked);
}

void ULoginWidget::Set() const
{
	LoginText->SetText(FText::FromString(FString("What the fuck")));
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ULoginWidget::OnLoginButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("FirstPersonMap"));
}
