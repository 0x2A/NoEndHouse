// Copyright (c) 2016 Stift15 Entertainment

#pragma once

#include "GameFramework/SaveGame.h"
#include "NEHSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class NOENDHOUSE_API UNEHSaveGame : public USaveGame
{
	GENERATED_BODY()
	
	
public:

	UNEHSaveGame();

	UPROPERTY(VisibleAnywhere, Category = Level)
	FString fLevelName;

	UPROPERTY(VisibleAnywhere, Category = Gameplay)
	FVector aPlayerLocation;

	UPROPERTY(VisibleAnywhere, Category = Gameplay)
	TArray<FString> inventory;

	UPROPERTY(VisibleAnywhere, Category = Gameplay)
		float Sanity;
	
};
