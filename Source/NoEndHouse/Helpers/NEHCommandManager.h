// Copyright (c) 2016 Stift15 Entertainment

#pragma once

#include "GameFramework/CheatManager.h"
#include "NEHCommandManager.generated.h"

/**
 * 
 */
UCLASS()
class NOENDHOUSE_API UNEHCommandManager : public UCheatManager
{
	GENERATED_BODY()
	
public:


	UFUNCTION(exec)
	virtual void GoToRoom(int roomNumber);
	
	
};
