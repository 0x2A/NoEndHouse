// Copyright (c) 2016 Stift15 Entertainment

#include "NoEndHouse.h"
#include "NEHGameState.h"
#include "Engine/Level.h"
#include "NoEndHouseCharacter.h"


ANEHGameState::ANEHGameState()
{
	LoadSaveGame(false);
	bPlayerDied = false;
	bDisclaimerRead = false;
}

bool ANEHGameState::SaveGame()
{
	static const auto CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("g.SaveGameEditorEnable"));
	auto worldType = GetWorld()->WorldType;
	if ((worldType == EWorldType::Editor || worldType == EWorldType::PIE) && CVar->GetInt() <= 0)
	{
		return false;
	}
	//we first need to check if we have a valid player character. Otherwise
	//there could be a save game call without a player character being chreated
	//resulting in an invalid player position saved
	auto tchar = GetWorld()->GetFirstPlayerController()->GetCharacter();
	ANoEndHouseCharacter* character = Cast<ANoEndHouseCharacter>(tchar);
	if (character)
	{
		if (!currentSaveGame)
			LoadSaveGame(true);

		currentSaveGame->fLevelName = GetWorld()->GetMapName();
		currentSaveGame->fLevelName.ReplaceInline(*GetWorld()->StreamingLevelsPrefix, TEXT(""));

		character->OnSaveGame(currentSaveGame);
		UGameplayStatics::SaveGameToSlot(currentSaveGame, "savegame", 0);
		return true;
	}
	return false;
}

UNEHSaveGame* ANEHGameState::LoadGame()
{
	LoadSaveGame(true);
	UGameplayStatics::OpenLevel(GetWorld(), FName(*currentSaveGame->fLevelName), true);
// 	ANoEndHouseCharacter* character = Cast<ANoEndHouseCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
// 	if (character)
// 	{
// 		character->SetActorLocation(currentSaveGame->aPlayerLocation);
// 		character->Inventory = currentSaveGame->inventory;
// 	}
	return currentSaveGame;
}

bool ANEHGameState::HasSaveGame()
{
	return UGameplayStatics::DoesSaveGameExist("savegame", 0);
}

void ANEHGameState::LoadSaveGame(bool createifNotPresent /*= false*/)
{
	currentSaveGame = Cast<UNEHSaveGame>(UGameplayStatics::LoadGameFromSlot("savegame", 0));
	if (!currentSaveGame && createifNotPresent)
		currentSaveGame = Cast<UNEHSaveGame>(UGameplayStatics::CreateSaveGameObject(UNEHSaveGame::StaticClass()));
}

void ANEHGameState::DeleteSaveGame()
{
	UGameplayStatics::DeleteGameInSlot("savegame", 0);
	currentSaveGame = nullptr;
}
