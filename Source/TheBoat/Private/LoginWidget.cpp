// It's made by Geon Oh

#include "LoginWidget.h"

#include "BoatGameInstance.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "HAL/PlatformProcess.h"
#include "Kismet/GameplayStatics.h"
#include "TheBoat/TheBoat.h"

void ULoginWidget::NativeConstruct()
{
	Super::NativeConstruct();
	LoginButton->OnClicked.AddUniqueDynamic(this, &ULoginWidget::OnLoginButtonClicked);

	if (UWorld* World = GetWorld())
	{
		if (UBoatGameInstance* BoatGameInstance = GetBoatGameInstance(World))
		{
			StatusMessageChangedHandle = BoatGameInstance->OnStatusMessageChanged().AddUObject(this, &ULoginWidget::HandleStatusMessageChanged);
			LoginSucceededHandle = BoatGameInstance->OnLoginSucceeded().AddUObject(this, &ULoginWidget::HandleLoginSucceeded);
		}
	}
}

void ULoginWidget::NativeDestruct()
{
	if (UWorld* World = GetWorld())
	{
		if (UBoatGameInstance* BoatGameInstance = Cast<UBoatGameInstance>(World->GetGameInstance()))
		{
			if (StatusMessageChangedHandle.IsValid())
			{
				BoatGameInstance->OnStatusMessageChanged().Remove(StatusMessageChangedHandle);
			}

			if (LoginSucceededHandle.IsValid())
			{
				BoatGameInstance->OnLoginSucceeded().Remove(LoginSucceededHandle);
			}
		}
	}

	Super::NativeDestruct();
}

void ULoginWidget::Set() const
{
	if (UWorld* World = GetWorld())
	{
		if (UBoatGameInstance* BoatGameInstance = GetBoatGameInstance(World))
		{
			LoginText->SetText(FText::FromString(BoatGameInstance->GetStatusMessage()));
		}
	}
}

void ULoginWidget::OnLoginButtonClicked()
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

	if (!BoatGameInstance->ConnectToServer())
	{
		return;
	}

	const FString PlayerName = FPlatformProcess::UserName(false);
	BoatGameInstance->SendHello(PlayerName);
}

void ULoginWidget::HandleStatusMessageChanged(const FString& InStatusMessage) const
{
	if (LoginText)
	{
		LoginText->SetText(FText::FromString(InStatusMessage));
	}
}

void ULoginWidget::HandleLoginSucceeded()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Lobby"));
}
