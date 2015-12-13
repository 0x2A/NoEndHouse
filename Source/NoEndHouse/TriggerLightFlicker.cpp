// Fill out your copyright notice in the Description page of Project Settings.

#include "NoEndHouse.h"
#include "TriggerLightFlicker.h"
#include "NoEndHouseCharacter.h"



ATriggerLightFlicker::ATriggerLightFlicker()
{
	TScriptDelegate<FWeakObjectPtr> delegateFunction;
	delegateFunction.BindUFunction(this, "CollisionOccured");

	GetCollisionComponent()->OnComponentBeginOverlap.Add(delegateFunction);
}

void ATriggerLightFlicker::CollisionOccured(AActor* actor)
{
	if (actor->IsA(ANoEndHouseCharacter::StaticClass()))
	{
		ANoEndHouseCharacter* character = Cast<ANoEndHouseCharacter>(actor);
	}
}
