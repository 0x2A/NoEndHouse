// Copyright (c) 2016 Stift15 Entertainment

#pragma once

#include "AIController.h"
#include "AIControllerMouse.generated.h"

/**
 * 
 */
UCLASS()
class NOENDHOUSE_API AAIControllerMouse : public AAIController
{
	GENERATED_BODY()
	
public:

	AAIControllerMouse();


	void BeginPlay() override;

	void Tick(float DeltaTime) override;

	void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result) override;


private:

	void MoveToRandomLocation();

	class ACharacterMouse* CharacterMouse;

	bool bMoving;
};
