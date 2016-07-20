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
	bool SaveGame();

	UFUNCTION(BlueprintCallable, Category = "Savegame")
	UNEHSaveGame* LoadGame();

	UFUNCTION(BlueprintCallable, Category = "Savegame")
		bool HasSaveGame();

	UFUNCTION(BlueprintCallable, Category = "Savegame")
		void DeleteSaveGame();

	UPROPERTY(BlueprintReadOnly, Category = "Savegame")
	UNEHSaveGame* currentSaveGame;

	UPROPERTY(BlueprintReadOnly, Category = "gameplay")
	bool bPlayerDied;



private:

	void LoadSaveGame(bool createifNotPresent = false);

};
