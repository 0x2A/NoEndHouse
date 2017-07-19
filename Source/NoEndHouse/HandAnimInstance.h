// Copyright (c) 2016 Stift15 Entertainment

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NEHMotionController.h"
#include "HandAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class NOENDHOUSE_API UHandAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	GripType GripState;
	
	
};
