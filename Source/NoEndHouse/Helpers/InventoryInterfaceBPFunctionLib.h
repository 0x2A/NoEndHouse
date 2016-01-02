// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "../InventoryInterface.h"
#include "InventoryInterfaceBPFunctionLib.generated.h"

/**
 * 
 */
UCLASS()
class NOENDHOUSE_API UInventoryInterfaceBPFunctionLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "InventoryInterface")
	static void AddInventory(const TScriptInterface<IInventoryInterface> &target, FString item);

	UFUNCTION(BlueprintCallable, Category = "InventoryInterface")
		static void RemoveInventory(const TScriptInterface<IInventoryInterface> &target, FString item);

	UFUNCTION(BlueprintCallable, Category = "InventoryInterface")
		static bool CheckInventory(const TScriptInterface<IInventoryInterface> &target, FString item);
};
