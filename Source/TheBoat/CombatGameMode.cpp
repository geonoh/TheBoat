// Copyright Epic Games, Inc. All Rights Reserved.

#include "CombatGameMode.h"

#include "CombatHUD.h"
#include "TheBoat.h"
#include "UObject/ConstructorHelpers.h"

ACombatGameMode::ACombatGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_CombatCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
}

void ACombatGameMode::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(ItemGenerateTimerHandle, this, &ACombatGameMode::OnItemGenerateStart, 5.f, true);
}

void ACombatGameMode::OnItemGenerateStart()
{
	ACombatHUD* CombatHUD = Cast<ACombatHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (!CombatHUD)
	{
		BOAT_LOG(Error, TEXT("CombatHUD did not create"));
		check(false);
		return;
	}

	CombatHUD->OnItemGenerateStart();
}
