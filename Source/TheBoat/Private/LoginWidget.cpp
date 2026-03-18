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

	if (LoginButton)
	{
		LoginButton->OnClicked.AddUniqueDynamic(this, &ULoginWidget::OnLoginButtonClicked);
	}

	if (PassthroughButton)
	{
		PassthroughButton->OnClicked.AddUniqueDynamic(this, &ULoginWidget::OnPassthroughButtonClicked);
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

	StatusMessageChangedHandle = BoatGameInstance->OnStatusMessageChanged().AddUObject(this, &ULoginWidget::HandleStatusMessageChanged);
	LoginSucceededHandle = BoatGameInstance->OnLoginSucceeded().AddUObject(this, &ULoginWidget::HandleLoginSucceeded);
}

void ULoginWidget::NativeDestruct()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		Super::NativeDestruct();
		return;
	}

	UBoatGameInstance* BoatGameInstance = Cast<UBoatGameInstance>(World->GetGameInstance());
	if (!BoatGameInstance)
	{
		Super::NativeDestruct();
		return;
	}

	if (StatusMessageChangedHandle.IsValid())
	{
		BoatGameInstance->OnStatusMessageChanged().Remove(StatusMessageChangedHandle);
	}

	if (LoginSucceededHandle.IsValid())
	{
		BoatGameInstance->OnLoginSucceeded().Remove(LoginSucceededHandle);
	}

	Super::NativeDestruct();
}

void ULoginWidget::Set() const
{
	if (!LoginText)
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

	LoginText->SetText(FText::FromString(BoatGameInstance->GetStatusMessage()));
}

void ULoginWidget::OnLoginButtonClicked()
{
	const UWorld* World = GetWorld();
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

void ULoginWidget::OnPassthroughButtonClicked()
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

	BoatGameInstance->SetPassthroughMode(true);
	UGameplayStatics::OpenLevel(World, TEXT("Lobby"));
}

void ULoginWidget::HandleStatusMessageChanged(const FString& InStatusMessage) const
{
	if (!LoginText)
	{
		return;
	}

	LoginText->SetText(FText::FromString(InStatusMessage));
}

void ULoginWidget::HandleLoginSucceeded()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Lobby"));
}
