// Copyright (c) 2016 Stift15 Entertainment

#include "NoEndHouse.h"
#include "InteractiveObject.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"


// Sets default values
// AObservableObject::AObservableObject()
// {
//  	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
// 	PrimaryActorTick.bCanEverTick = true;
// 	bObserving = false;
// 
// 	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
// 	Mesh->AttachParent = GetRootComponent();
// 	Mesh->SetCollisionResponseToChannel(COLLISION_OBSERVABLE, ECR_Block);
// 
// 	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
// 	
// }

UInteractiveObject::UInteractiveObject(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

