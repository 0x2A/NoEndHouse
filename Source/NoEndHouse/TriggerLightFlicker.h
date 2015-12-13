// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/TriggerBox.h"
#include "TriggerLightFlicker.generated.h"

/**
 * 
 */
UCLASS()
class NOENDHOUSE_API ATriggerLightFlicker : public ATriggerBox
{
	GENERATED_BODY()

public:
	ATriggerLightFlicker();
	void CollisionOccured(AActor* actor);

	UFUNCTION(BlueprintImplementableEvent, Category = Trigger)
	void OnDoFlicker();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Trigger)
	TArray<AActor*> Lights;
};
