// It's made by Geon Oh

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginWidget.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class THEBOAT_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	void Set() const;

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* LoginButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LoginText;

private:
	void HandleStatusMessageChanged(const FString& InStatusMessage) const;
	void HandleLoginSucceeded();

	UFUNCTION()
	void OnLoginButtonClicked();

	FDelegateHandle StatusMessageChangedHandle;
	FDelegateHandle LoginSucceededHandle;
};
