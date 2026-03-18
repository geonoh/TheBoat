// Copyright Epic Games, Inc. All Rights Reserved.

#include "CombatCharacter.h"

#include "Animation/AnimInstance.h"
#include "BoatGameInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "TheBoat.h"
#include "../../Shared/Defines.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ACombatCharacter::ACombatCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	CharacterId = 0;
	LastSentNetworkPosition = FVector::ZeroVector;
	MoveSyncElapsedTime = 0.0f;
	PrimaryActorTick.bCanEverTick = true;
}

void ACombatCharacter::InitCharacter(const FCombatCharacterInfo& Info)
{
	CharacterId = Info.CharacterId;
}

int64 ACombatCharacter::GetCharacterId() const
{
	return CharacterId;
}

void ACombatCharacter::BeginPlay()
{
	Super::BeginPlay();
	LastSentNetworkPosition = GetActorLocation();
}

void ACombatCharacter::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);

	if (!IsLocallyControlled())
	{
		return;
	}

	MoveSyncElapsedTime += InDeltaTime;
	if (MoveSyncElapsedTime < 0.1f)
	{
		return;
	}

	MoveSyncElapsedTime = 0.0f;

	const FVector CurrentLocation = GetActorLocation();
	if (FVector::DistSquared(CurrentLocation, LastSentNetworkPosition) < 25.0f)
	{
		return;
	}

	UBoatGameInstance* BoatGameInstance = GetBoatGameInstance(GetWorld());
	if (!BoatGameInstance)
	{
		return;
	}

	if (BoatGameInstance->SendMove(CurrentLocation))
	{
		LastSentNetworkPosition = CurrentLocation;
	}
}

void ACombatCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInputComponent)
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
		return;
	}

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACombatCharacter::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACombatCharacter::Look);
}

void ACombatCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller == nullptr)
	{
		return;
	}

	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
	AddMovementInput(GetActorRightVector(), MovementVector.X);
}

void ACombatCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller == nullptr)
	{
		return;
	}

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}
