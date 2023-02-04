// Copyright Epic Games, Inc. All Rights Reserved.

#include "GGJ2023GameMode.h"
#include "GGJ2023PlayerController.h"
#include "GGJ2023Character.h"
#include "UObject/ConstructorHelpers.h"

AGGJ2023GameMode::AGGJ2023GameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AGGJ2023PlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}