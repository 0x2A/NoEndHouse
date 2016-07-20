// Copyright (c) 2016 Stift15 Entertainment

#include "NoEndHouse.h"
#include "GameplayBPFunctionLibrary.h"

void UGameplayBPFunctionLibrary::GetRenderedActors(TArray<AActor*>& CurrentlyRenderedActors, float MinRecentTime)
{
	//Empty any previous entries
	CurrentlyRenderedActors.Empty();

	//Iterate Over Actors
	for (TObjectIterator<AActor> Itr; Itr; ++Itr)
	{
		if (Itr->GetWorld()->TimeSeconds - Itr->GetLastRenderTime() <= MinRecentTime)
		{
			CurrentlyRenderedActors.Add(*Itr);
		}
	}
}

void UGameplayBPFunctionLibrary::GetNotRenderedActors(TArray<AActor*>& CurrentlyNotRenderedActors, float MinRecentTime)
{
	//Empty any previous entries
	CurrentlyNotRenderedActors.Empty();

	//Iterate Over Actors
	for (TObjectIterator<AActor> Itr; Itr; ++Itr)
	{
		if (Itr->GetWorld()->TimeSeconds - Itr->GetLastRenderTime() > MinRecentTime)
		{
			CurrentlyNotRenderedActors.Add(*Itr);
		}
	}
}

bool UGameplayBPFunctionLibrary::ActorVisible(AActor* TargetActor, float MinRecentTime)
{
	float t = TargetActor->GetWorld()->TimeSeconds - TargetActor->GetLastRenderTime();
	return t <= MinRecentTime;
}

float UGameplayBPFunctionLibrary::GetLastRenderTime(AActor* TargetActor)
{
	return TargetActor->GetWorld()->TimeSeconds - TargetActor->GetLastRenderTime();
}
