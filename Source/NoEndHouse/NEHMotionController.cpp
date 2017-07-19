// Copyright (c) 2016 Stift15 Entertainment

#include "NoEndHouse.h"
#include "MotionControllerComponent.h"
#include "SteamVRChaperoneComponent.h"
#include "InteractiveObject.h"
#include "HandAnimInstance.h"
#include "NEHMotionController.h"


// Sets default values
ANEHMotionController::ANEHMotionController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
	
	HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMesh"));
	HandMesh->AttachToComponent(MotionController, FAttachmentTransformRules::KeepWorldTransform);
	HandMesh->SetRelativeRotation(FQuat::MakeFromEuler(FVector(90, 0, 0)));
	GrabSphere = CreateDefaultSubobject<USphereComponent>(TEXT("GrabSphere"));
	GrabSphere->AttachToComponent(HandMesh, FAttachmentTransformRules::KeepRelativeTransform);
	GrabSphere->OnComponentBeginOverlap.AddDynamic(this, &ANEHMotionController::OnGrabSphereOverlapBegin);

	SteamVRChaperone = CreateDefaultSubobject<USteamVRChaperoneComponent>(TEXT("SteamVRChaperone"));
	SteamVRChaperone->bAutoActivate = true;

	AttachedActor = NULL;

	bAlwaysCollideHands = true;

}



// Called when the game starts or when spawned
void ANEHMotionController::BeginPlay()
{
	Super::BeginPlay();
}


void ANEHMotionController::UpdateHandAnimation()
{
	if (AttachedActor != NULL || bWantsToGrip)
	{
		GripState = GripType::GT_Grab;
	}
	else
	{
		auto actor = GetActorNearHand();
		if (actor != NULL)
		{
			GripState = GripType::GT_CanGrab;
		}
		else
		{
			GripState = bWantsToGrip ? GripType::GT_Grab : GripType::GT_Open;
		}
	}

	
	if (HandMesh != NULL)
	{
		UHandAnimInstance* inst = Cast<UHandAnimInstance>(HandMesh->GetAnimInstance());
		if (inst != NULL)
		{
			inst->GripState = GripState;
		}
	}

	//Only let hand collide with environment while gripping 
	if (!bAlwaysCollideHands)
	{
		switch (GripState)
		{
		case GripType::GT_CanGrab:
		case GripType::GT_Open:
			HandMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			break;
		case GripType::GT_Grab:
			HandMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			break;
		}
	}
}

AActor* ANEHMotionController::GetActorNearHand()
{

	float nearestOverlap = 10000.0f;
	AActor* nearestOverlappingActor = NULL;
	TArray<AActor*> overlappingActors;
	GrabSphere->GetOverlappingActors(overlappingActors, AActor::StaticClass());
	FVector sphereLocation = GrabSphere->GetComponentLocation();

	for (AActor* actor : overlappingActors)
	{
		if (actor->GetClass()->ImplementsInterface(UInteractiveObject::StaticClass()) || actor->GetComponentsCollisionResponseToChannel(COLLISION_OBSERVABLE) == ECR_Block)
		{
			auto actorLocation = actor->GetActorLocation();
			auto dist = actorLocation - sphereLocation;
			if (dist.Size() < nearestOverlap)
			{
				nearestOverlappingActor = actor;
				nearestOverlap = dist.Size();
			}
		}
	}
	return nearestOverlappingActor;
}

void ANEHMotionController::OnGrabSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto other = Cast<UStaticMeshComponent>(OtherComp);
	if (other != NULL)
	{
		if (other->IsSimulatingPhysics())
			RumbleController();
	}
}

// Called every frame
void ANEHMotionController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateHandAnimation();
}

void ANEHMotionController::GrabActor()
{
	bWantsToGrip = true;
	auto actor = GetActorNearHand();
	if (IsValid(actor))
	{
		AttachedActor = actor;
		if (actor->GetClass()->ImplementsInterface(UInteractiveObject::StaticClass()) && IInteractiveObject::Execute_CanPickup(actor))
		{
			IInteractiveObject::Execute_BeginInteraction(actor);
		}

		//actor->DisableComponentsSimulatePhysics();
		FAttachmentTransformRules attchmtRule(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);
		actor->GetRootComponent()->AttachToComponent(RootComponent, attchmtRule);
	}
}

void ANEHMotionController::SetHand(EControllerHand hand, USkeletalMesh* mesh, TSubclassOf<UAnimInstance> anim)
{
	Hand = hand;
	MotionController->Hand = Hand;
	MotionController->SetupAttachment(RootComponent);

	HandMesh->SetSkeletalMesh(mesh);
	HandMesh->AnimClass = anim;

	switch (Hand)
	{
	case EControllerHand::Left:
		HandMesh->SetWorldScale3D(FVector(1, 1, -1));
		break;
	}

	if (bAlwaysCollideHands)
		HandMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ANEHMotionController::ReleaseActor()
{
	bWantsToGrip = false;
	if (IsValid(AttachedActor))
	{
		if (AttachedActor->GetRootComponent()->GetAttachParent() == RootComponent)
		{
			FDetachmentTransformRules rules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false);
			AttachedActor->DetachFromActor(rules);
			RumbleController(0.2f);
		}
		AttachedActor = nullptr;
	}
}

void ANEHMotionController::RumbleController(float scale)
{
	if (HapticEffect != NULL)
	{
		GetWorld()->GetFirstPlayerController()->PlayHapticEffect(HapticEffect, Hand, scale);
	}
}

