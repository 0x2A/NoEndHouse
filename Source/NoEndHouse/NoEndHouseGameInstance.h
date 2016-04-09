// Copyright (c) 2016 Stift15 Entertainment

#pragma once

#include "Engine/GameInstance.h"
#include "NoEndHouseGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class NOENDHOUSE_API UNoEndHouseGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	virtual void Init() override;
	
	UFUNCTION(BlueprintCallable, category = UI)
	virtual void BeginLoadingScreen();
	UFUNCTION()
	virtual void EndLoadingScreen();

private:

	bool bFirstLoaded;
};
