// It's made by Geon Oh

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class THEBOAT_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void Set();

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* StartGame;

private:
	UFUNCTION()
	void OnStartGameButtonClicked();
};
