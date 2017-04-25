// Fill out your copyright notice in the Description page of Project Settings.

#include "NoEndHouse.h"
#include "TriggerLightFlicker.h"
#include "NoEndHouseCharacter.h"


const float ATriggerLightFlicker::DELTATIME = 0.01f;

ATriggerLightFlicker::ATriggerLightFlicker()
{
	Sound = CreateDefaultSubobject<UAudioComponent>(TEXT("Sound"));
	Sound->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	Sound->bIsMusic = false;
	Sound->bOverrideAttenuation = true;

	GetCollisionComponent()->bGenerateOverlapEvents = true;

	bPlayOnce = true;

	TimeLine = FTimeline();

}


void ATriggerLightFlicker::TimelineProgress(float Value)
{
	if (character)
	{
		character->GetFirstPersonCameraComponent()->PostProcessSettings.bOverride_IndirectLightingIntensity = true;
		character->GetFirstPersonCameraComponent()->PostProcessSettings.IndirectLightingIntensity = Value;
	}

	for (int i = 0; i < LightComponents.Num(); i++)
	{
		LightComponents[i]->SetIntensity(BaseLightIntensities[i] * Value);
	}
}

void ATriggerLightFlicker::TickTimeline()
{
	if (TimeLine.IsPlaying())
	{
		TimeLine.TickTimeline(DELTATIME);
	}
	else
	{
		if (bPlayOnce)
		{
			GetWorldTimerManager().ClearTimer(TimerHandle);
			Destroy();
		}
	}
}

void ATriggerLightFlicker::BeginPlay()
{
	Super::BeginPlay();


	if (Curve)
	{
		FOnTimelineFloat progressFunction{};
		progressFunction.BindUFunction(this, "TimelineProgress"); // The function EffectProgress gets called
		TimeLine.AddInterpFloat(Curve, progressFunction, FName{ TEXT("Effect") });
	}

	//fetch all lights and save base light intensity
	for (int i = 0; i < Lights.Num(); i++)
	{
		ULightComponent* light = Cast<ULightComponent>(Lights[i]->GetComponentByClass(ULightComponent::StaticClass()));
		if (!light) continue;
		LightComponents.Add(light);
		BaseLightIntensities.Add(light->Intensity);
	}
}

void ATriggerLightFlicker::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);


	if (OtherActor && (OtherActor != this) && OtherActor->IsA(ANoEndHouseCharacter::StaticClass()))
	{
		character = Cast<ANoEndHouseCharacter>(OtherActor);
		TimeLine.PlayFromStart();
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ATriggerLightFlicker::TickTimeline, DELTATIME, true, 0.0f);
		Sound->Play();
	}

}


