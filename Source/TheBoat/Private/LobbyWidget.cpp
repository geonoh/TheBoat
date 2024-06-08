// It's made by Geon Oh


#include "LobbyWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	StartGame->OnClicked.AddUniqueDynamic(this, &ULobbyWidget::OnStartGameButtonClicked);
}

void ULobbyWidget::Set()
{
	// Gunny TODO
	// 1. Show rooms
	// 2. Character customize

}

void ULobbyWidget::OnStartGameButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("FirstPersonMap"));

}
