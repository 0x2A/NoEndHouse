// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once 
#include "GameFramework/HUD.h"
#include "NoEndHouseHUD.generated.h"

UCLASS()
class ANoEndHouseHUD : public AHUD
{
	GENERATED_BODY()

public:
	ANoEndHouseHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:

};

