// Fill out your copyright notice in the Description page of Project Settings.

#include "NoEndHouse.h"
#include "InventoryInterface.h"

UInventoryInterface::UInventoryInterface(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

bool IInventoryInterface::CheckInventory(FString item)
{
	return false;
}

void IInventoryInterface::AddInventory(FString item)
{

}

void IInventoryInterface::RemoveInventory(FString item)
{

}
