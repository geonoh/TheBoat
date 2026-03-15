// It's made by Geon Oh

#include "LobbyWidget.h"

#include "BoatGameInstance.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "TheBoat/TheBoat.h"

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	StartGame->OnClicked.AddUniqueDynamic(this, &ULobbyWidget::OnStartGameButtonClicked);

	if (UWorld* World = GetWorld())
	{
		if (UBoatGameInstance* BoatGameInstance = GetBoatGameInstance(World))
		{
			MatchCreatedHandle = BoatGameInstance->OnMatchCreated().AddUObject(this, &ULobbyWidget::HandleMatchCreated);
		}
	}
}

void ULobbyWidget::NativeDestruct()
{
	if (UWorld* World = GetWorld())
	{
		if (UBoatGameInstance* BoatGameInstance = Cast<UBoatGameInstance>(World->GetGameInstance()))
		{
			if (MatchCreatedHandle.IsValid())
			{
				BoatGameInstance->OnMatchCreated().Remove(MatchCreatedHandle);
			}
		}
	}

	Super::NativeDestruct();
}

void ULobbyWidget::Set()
{
	if (UWorld* World = GetWorld())
	{
		if (UBoatGameInstance* BoatGameInstance = GetBoatGameInstance(World))
		{
			StartGame->SetIsEnabled(BoatGameInstance->IsLoggedInToBoatServer());
		}
	}
}

void ULobbyWidget::OnStartGameButtonClicked()
{
	if (UWorld* World = GetWorld())
	{
		if (UBoatGameInstance* BoatGameInstance = GetBoatGameInstance(World))
		{
			BoatGameInstance->SendQueueJoin();
		}
	}
}

void ULobbyWidget::HandleMatchCreated(int64 InMatchId)
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("FirstPersonMap"));
}
