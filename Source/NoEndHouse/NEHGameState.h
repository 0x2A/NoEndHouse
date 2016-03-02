// Copyright (c) 2016 Stift15 Entertainment

#pragma once

#include "GameFramework/GameState.h"
#include "NEHSaveGame.h"
#include "NEHGameState.generated.h"

/**
 * 
 */
UCLASS()
class NOENDHOUSE_API ANEHGameState : public AGameState
{
	GENERATED_BODY()

public:
	ANEHGameState();
	
	UFUNCTION(BlueprintCallable, Category = "Savegame")
	void SaveGame();

	UFUNCTION(BlueprintCallable, Category = "Savegame")
	UNEHSaveGame* LoadGame();


private:

	UNEHSaveGame* currentSaveGame;
};
