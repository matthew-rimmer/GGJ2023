// Copyright Epic Games, Inc. All Rights Reserved.

#include "GGJ2023PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "GGJ2023Character.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include <Kismet/KismetMathLibrary.h>

AGGJ2023PlayerController::AGGJ2023PlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void AGGJ2023PlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AGGJ2023PlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AGGJ2023PlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AGGJ2023PlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AGGJ2023PlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AGGJ2023PlayerController::OnSetDestinationReleased);

		EnhancedInputComponent->BindAction(MoveUpAction, ETriggerEvent::Triggered, this, &AGGJ2023PlayerController::OnMoveUp);
		EnhancedInputComponent->BindAction(MoveDownAction, ETriggerEvent::Triggered, this, &AGGJ2023PlayerController::OnMoveDown);
		EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Triggered, this, &AGGJ2023PlayerController::OnMoveLeft);
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &AGGJ2023PlayerController::OnMoveRight);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AGGJ2023PlayerController::OnShoot);
	}
}

void AGGJ2023PlayerController::OnInputStarted()
{
	StopMovement();
}

void AGGJ2023PlayerController::OnMoveUp() {
	APawn* ControlledPawn = GetPawn();
	ControlledPawn->AddMovementInput(FVector(25, 0, 0));
}

void AGGJ2023PlayerController::OnMoveDown() {
	APawn* ControlledPawn = GetPawn();
	ControlledPawn->AddMovementInput(FVector(-25, 0, 0));
}

void AGGJ2023PlayerController::OnMoveLeft() {
	APawn* ControlledPawn = GetPawn();
	ControlledPawn->AddMovementInput(FVector(0, -25, 0));
}

void AGGJ2023PlayerController::OnMoveRight() {
	APawn* ControlledPawn = GetPawn();
	ControlledPawn->AddMovementInput(FVector(0, 25, 0));
}

void AGGJ2023PlayerController::OnShoot() {
	APawn* ControlledPawn = GetPawn();

	FHitResult Hit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(ControlledPawn);
	GetWorld()->LineTraceSingleByChannel(Hit, ControlledPawn->GetActorLocation(), (ControlledPawn->GetActorLocation() + ControlledPawn->GetActorForwardVector() * 1000.0f), ECC_MAX, QueryParams);

	if (Hit.bBlockingHit && IsValid(Hit.GetActor())) {
		DrawDebugLine(GetWorld(), ControlledPawn->GetActorLocation(), Hit.Location, FColor::Emerald, true, -1, 0, 10);
		FString name = Hit.GetActor()->GetActorNameOrLabel();
		UE_LOG(LogTemp, Warning, TEXT("%s"), *name);
	}
}

// Triggered every frame when the input is held down
void AGGJ2023PlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();

	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;

	bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}

	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FRotator lookAtRotation = UKismetMathLibrary::FindLookAtRotation(ControlledPawn->GetActorLocation(), CachedDestination);
		ControlledPawn->SetActorRotation(FRotator(0, lookAtRotation.Yaw, lookAtRotation.Roll));
	}
}

void AGGJ2023PlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	//if (FollowTime <= ShortPressThreshold)
	//{
	//	// We move there and spawn some particles
	//	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
	//	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	//}

	//FollowTime = 0.f;
}
