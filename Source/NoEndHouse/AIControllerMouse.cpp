// Copyright (c) 2016 Stift15 Entertainment

#include "NoEndHouse.h"
#include "AIControllerMouse.h"
#include "CharacterMouse.h"


AAIControllerMouse::AAIControllerMouse()
{
	bAllowStrafe = false;
	
	bMoving = false;
}

void AAIControllerMouse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!CharacterMouse) return;	

	if (!CharacterMouse->ShouldMove && bMoving)
	{
		bMoving = false;
		StopMovement();
		return;
	}
	else if (CharacterMouse->ShouldMove && !bMoving)
	{
		MoveToRandomLocation();
	}
}

void AAIControllerMouse::BeginPlay()
{
	Super::BeginPlay();

	CharacterMouse = Cast<ACharacterMouse>(GetCharacter());
}

void AAIControllerMouse::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (CharacterMouse->ShouldMove)
		MoveToRandomLocation();
}

void AAIControllerMouse::MoveToRandomLocation()
{
	bMoving = true;

	auto locations = CharacterMouse->TargetPositions;
	int indx = FMath::RandRange(0, locations.Num() - 1);

	auto location = locations[indx];
	MoveToLocation(location->GetActorLocation(), 1.0f, true, true, false, false);
}
