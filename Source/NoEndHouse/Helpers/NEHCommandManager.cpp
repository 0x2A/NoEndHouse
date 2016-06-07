// Copyright (c) 2016 Stift15 Entertainment

#include "NoEndHouse.h"
#include "NEHCommandManager.h"


static TAutoConsoleVariable<int32> CVarRefractionQuality(
	TEXT("g.SaveGameEditorEnable"),
	0,
	TEXT("Whether or not use the save game when in editor or debugging mode\n")
	TEXT("0: don't save when in editor or debugging mode\n")
	TEXT("1: always save game\n"),
	ECVF_SetByConsole | ECVF_Cheat); //using ECVF_Cheat causes the command to be removed from shipping build


void UNEHCommandManager::GoToRoom(int roomNumber)
{
	UE_LOG(LogTemp, Warning, TEXT("Command GoToRoom not implemented yet"));
}
