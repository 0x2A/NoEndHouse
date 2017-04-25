// Copyright (c) 2016 Stift15 Entertainment

#include "NoEndHouse.h"
#include "AIControllerMouse.h"
#include "CharacterMouse.h"
#include "Engine/World.h"
#include "Engine/Level.h"


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
	if (CharacterMouse && CharacterMouse->TargetPositions.Num() == 0)
		CharacterMouse->DisableMovement();
}

void AAIControllerMouse::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	switch (Result)
	{
	case EPathFollowingResult::Blocked:
	case EPathFollowingResult::Success:
		if (CharacterMouse->ShouldMove)
			MoveToRandomLocation();
		break;
	case EPathFollowingResult::OffPath:
		break;
	case EPathFollowingResult::Aborted:
		bMoving = false;
		break;
	case EPathFollowingResult::Invalid:
		bMoving = false;
		break;
	}
}

void AAIControllerMouse::MoveToRandomLocation()
{
	bMoving = true;

	auto locations = CharacterMouse->TargetPositions;
	if (locations.Num() == 0) return;

	int indx = FMath::RandRange(0, locations.Num() - 1);

	auto location = locations[indx];
	MoveToLocation(location->GetActorLocation(), 1.0f, true, true, false, false);
}
