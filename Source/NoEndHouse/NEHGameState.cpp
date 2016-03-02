// Copyright (c) 2016 Stift15 Entertainment

#include "NoEndHouse.h"
#include "NEHGameState.h"





ANEHGameState::ANEHGameState()
{
	currentSaveGame = nullptr;
}

void ANEHGameState::SaveGame()
{

}

UNEHSaveGame* ANEHGameState::LoadGame()
{
	return Cast<UNEHSaveGame>(UGameplayStatics::CreateSaveGameObject(UNEHSaveGame::StaticClass()));
}
