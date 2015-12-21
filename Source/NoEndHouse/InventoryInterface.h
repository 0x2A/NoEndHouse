// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InventoryInterface.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI)
class UInventoryInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()

};

class IInventoryInterface
{
	GENERATED_IINTERFACE_BODY()

public:

	void AddInventory(FString item);
	void RemoveInventory(FString item);
	bool CheckInventory(FString item);

};