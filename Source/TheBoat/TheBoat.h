// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BoatGameInstance.h"

DECLARE_LOG_CATEGORY_EXTERN(BOAT_LOG, Log, All);

#define BOAT_LOG(level, content) UE_LOG(BOAT_LOG, level, content)

UBoatGameInstance* GetBoatGameInstance(const UWorld* World);