// It's made by Geon Oh

#include "LobbyWidget.h"

#include "BoatGameInstance.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "TheBoat/TheBoat.h"

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!StartGame)
	{
		return;
	}

	StartGame->OnClicked.AddUniqueDynamic(this, &ULobbyWidget::OnStartGameButtonClicked);

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	UBoatGameInstance* BoatGameInstance = GetBoatGameInstance(World);
	if (!BoatGameInstance)
	{
		return;
	}

	MatchCreatedHandle = BoatGameInstance->OnMatchCreated().AddUObject(this, &ULobbyWidget::HandleMatchCreated);
}

void ULobbyWidget::NativeDestruct()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		Super::NativeDestruct();
		return;
	}

	UBoatGameInstance* BoatGameInstance = Cast<UBoatGameInstance>(World->GetGameInstance());
	if (!BoatGameInstance || !MatchCreatedHandle.IsValid())
	{
		Super::NativeDestruct();
		return;
	}

	BoatGameInstance->OnMatchCreated().Remove(MatchCreatedHandle);
	Super::NativeDestruct();
}

void ULobbyWidget::Set()
{
	if (!StartGame)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	UBoatGameInstance* BoatGameInstance = GetBoatGameInstance(World);
	if (!BoatGameInstance)
	{
		return;
	}

	const bool bIsLoggedIn = BoatGameInstance->IsLoggedInToBoatServer();
	const bool bIsPassthroughMode = BoatGameInstance->IsPassthroughMode();
	StartGame->SetIsEnabled(bIsLoggedIn || bIsPassthroughMode);
}

void ULobbyWidget::OnStartGameButtonClicked()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	UBoatGameInstance* BoatGameInstance = GetBoatGameInstance(World);
	if (!BoatGameInstance)
	{
		return;
	}

	if (BoatGameInstance->IsPassthroughMode())
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("FirstPersonMap"));
	}
	else
	{
		BoatGameInstance->SendQueueJoin();
	}
}

void ULobbyWidget::HandleMatchCreated(int64 InMatchId)
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("FirstPersonMap"));
}
