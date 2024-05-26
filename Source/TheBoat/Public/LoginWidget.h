// It's made by Geon Oh

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginWidget.generated.h"

class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class THEBOAT_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	void Set();

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* LoginButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LoginText;
};
