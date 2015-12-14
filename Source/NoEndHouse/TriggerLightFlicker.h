// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/TriggerBox.h"
#include "Components/TimelineComponent.h"
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
	

	/** Function which gets called from the Timer to call EffectProgress */
	void TickTimeline();

	/** Function which gets called from the Timeline on a Tick */
	UFUNCTION()
	void TimelineProgress(float Value);

	void BeginPlay();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Trigger)
	TArray<AActor*> Lights;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Trigger)
		bool bPlayOnce;


	UPROPERTY(EditAnywhere, Category = Trigger)
	class UCurveFloat* Curve;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sound, meta = (AllowPrivateAccess = "true"))
	class UAudioComponent* Sound;

	/** Deltatime(stepsize) for the timer calling the timeline tick */
	static const float DELTATIME;

	/** Timeline for the effectprogress*/
	FTimeline TimeLine;

	FTimerHandle TimerHandle;

	class ANoEndHouseCharacter* character;
	TArray<float> BaseLightIntensities;
	TArray<ULightComponent*> LightComponents;
};
