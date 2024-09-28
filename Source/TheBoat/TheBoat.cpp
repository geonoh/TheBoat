// Copyright Epic Games, Inc. All Rights Reserved.

#include "TheBoat.h"

#include "BoatGameInstance.h"
#include "Producer.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, TheBoat, "TheBoat" );

DEFINE_LOG_CATEGORY(BOAT_LOG);

UBoatGameInstance* GetBoatGameInstance(const UWorld* World)
{
	if (!World)
	{
		check(false);
	}
	
	UBoatGameInstance* RetVal = Cast<UBoatGameInstance>(World->GetGameInstance());
	return RetVal;
}

UProducer& GetProducer()
{
	return UProducer::Get();
}
