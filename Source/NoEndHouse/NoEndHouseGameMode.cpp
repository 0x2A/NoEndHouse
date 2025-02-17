// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "NoEndHouse.h"
#include "NoEndHouseGameMode.h"
#include "NoEndHouseHUD.h"
#include "NoEndHouseCharacter.h"
#include "NEHPlayerController.h"
#include "NEHGameState.h"

ANoEndHouseGameMode::ANoEndHouseGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Characters/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ANoEndHouseHUD::StaticClass();
	GameStateClass = ANEHGameState::StaticClass();
	PlayerControllerClass = ANEHPlayerController::StaticClass();
}

