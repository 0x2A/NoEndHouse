// Copyright (c) 2016 Stift15 Entertainment

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayBPFunctionLibrary.generated.h"
/**
 * 
 */
UCLASS()
class NOENDHOUSE_API UGameplayBPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "Gameplay|Visibility", meta = (WorldContext = "WorldContextObject"))
		static	void GetRenderedActors(TArray<AActor*>& CurrentlyRenderedActors, float MinRecentTime = 0.03f);

	UFUNCTION(BlueprintCallable, Category = "Gameplay|Visibility", meta = (WorldContext = "WorldContextObject"))
		static void GetNotRenderedActors(TArray<AActor*>& CurrentlyNotRenderedActors, float MinRecentTime = 0.03f);

	UFUNCTION(BlueprintCallable, Category = "Gameplay|Visibility", meta = (WorldContext = "WorldContextObject"))
		static bool ActorVisible(AActor* TargetActor, float MinRecentTime = 0.03f);

	UFUNCTION(BlueprintCallable, Category = "Gameplay|Visibility", meta = (WorldContext = "WorldContextObject"))
		static float GetLastRenderTime(AActor* TargetActor);
};
