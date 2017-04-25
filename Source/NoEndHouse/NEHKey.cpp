// Copyright (c) 2016 Stift15 Entertainment

#include "NoEndHouse.h"
#include "NEHKey.h"
#include "NoEndHouseCharacter.h"

// Sets default values
ANEHKey::ANEHKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	KeyID = "INVALID_KEYID";

	KeyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMesh"));
	KeyMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	KeyMesh->SetSimulatePhysics(false);
	
	
	DissolveParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DissolveParticleEffect"));
	DissolveParticleEffect->AttachToComponent(KeyMesh, FAttachmentTransformRules::KeepWorldTransform);
	bPickedUp = false;
}

// Called when the game starts or when spawned
void ANEHKey::BeginPlay()
{
	Super::BeginPlay();
	DissolveParticleEffect->DeactivateSystem();

	for (int i = 0; i < KeyMesh->GetNumMaterials(); i++)
	{
		MaterialInstances.Add(KeyMesh->CreateDynamicMaterialInstance(i, KeyMesh->GetMaterial(i)));
	}
}

// Called every frame
void ANEHKey::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ANEHKey::PickUp()
{
	if (KeyID.Equals("INVALID_KEYID"))
		return;

	ANoEndHouseCharacter* character = Cast<ANoEndHouseCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (character)
	{
		if (!character->CheckInventory(KeyID))
			character->AddInventory(KeyID);
	}
	bPickedUp = true;
	KeyMesh->SetSimulatePhysics(true);
	KeyMesh->SetEnableGravity(false);
	KeyMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	KeyMesh->AddImpulse(FVector(0, 0.5f, 1.5f));
	KeyMesh->AddTorque(FVector(-90, 1, 2));
	DissolveParticleEffect->ActivateSystem(true);

	OnPickUp();
	//FTimerManager::SetTimer()
}

void ANEHKey::BeginInteraction_Implementation()
{
	PickUp();
}


bool ANEHKey::CanPickup_Implementation()
{
	return false;
}

bool ANEHKey::AllowInteraction_Implementation()
{
	return !bPickedUp;
}

void ANEHKey::ObservingChangedRotation_Implementation(FRotator newRotation)
{
	//nothing to do here
}

void ANEHKey::BeginObservation_Implementation()
{
	//nothing to do here
}

void ANEHKey::EndObservation_Implementation()
{
	//nothing to do here
}

