// Copyright (c) 2016 Stift15 Entertainment

#include "NoEndHouse.h"
#include "CharacterMouse.h"
#include "EngineUtils.h"


// Sets default values
ACharacterMouse::ACharacterMouse()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	MouseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MouseMesh"));
	MouseMesh->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepWorldTransform);
	MouseMesh->bCastDynamicShadow = true;
	MouseMesh->CastShadow = true;

	RatSound = CreateDefaultSubobject<UAudioComponent>(TEXT("RatSound"));
	RatSound->bAutoActivate = false;

	bUseControllerRotationYaw = false;

	ShouldMove = true;

}

// Called when the game starts or when spawned
void ACharacterMouse::BeginPlay()
{
	Super::BeginPlay();
	
	if (ShouldMove)
		RatSound->Play();

	TargetPositions.Empty();
	for (TActorIterator<ATargetPoint> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr->GetName().StartsWith(WaypointPrefix))
			TargetPositions.Add(*ActorItr);
	}
}

// Called every frame
void ACharacterMouse::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void ACharacterMouse::DisableMovement()
{
	Controller->StopMovement();
	ShouldMove = false;
	RatSound->Stop();
}

void ACharacterMouse::EnableMovement()
{
	ShouldMove = true;
	RatSound->Play();
}



