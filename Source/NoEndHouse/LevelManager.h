// Copyright (c) 2016 Stift15 Entertainment

#pragma once

#include "Object.h"
#include "LevelManager.generated.h"

/**
 * 
 */
UCLASS()
class NOENDHOUSE_API ULevelManager : public UObject
{
	GENERATED_BODY()
	
public:

	/**
	*	Loads the targetLevel and prepares the NextLevel
	*	Useful for initial level loading, ie new game or loading game
	*
	* @param       PreviousLevel  Name of the previous level, empty when none
	* @param       targetLevel  target level to place player in
	* @param       nextLevel  next level to load, empty when none
	*/
	UFUNCTION(BlueprintCallable, Category = Level)
	static void PrepareLevel(FString previousLevel, FString targetLevel, FString NextLevel);

	/**
	*	Transitions to the next registered level and removes the previous one, 
	*	setting the old current level as old and sets the input level as next
	*
	* @param       nextnextLevel  level to set as new next level
	*/
	UFUNCTION(BlueprintCallable, Category = Level)
	static void EnterNextLevel(FString nextNextLevel);

	static class ULevel* uNextLevel;
	static class ULevel* uCurrentLevel;
	static class ULevel* uPreviousLevel;

	
};
