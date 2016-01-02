// Fill out your copyright notice in the Description page of Project Settings.

#include "NoEndHouse.h"
#include "InventoryInterfaceBPFunctionLib.h"




void UInventoryInterfaceBPFunctionLib::AddInventory(const TScriptInterface<IInventoryInterface> &target, FString item)
{
	target->AddInventory(item);
}


void UInventoryInterfaceBPFunctionLib::RemoveInventory(const TScriptInterface<IInventoryInterface> &target, FString item)
{
	target->RemoveInventory(item);
}

bool UInventoryInterfaceBPFunctionLib::CheckInventory(const TScriptInterface<IInventoryInterface> &target, FString item)
{
	return target->CheckInventory(item);
}
