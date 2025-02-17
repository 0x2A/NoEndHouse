// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "VRLeverComponent.h"

  //=============================================================================
UVRLeverComponent::UVRLeverComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	this->bGenerateOverlapEvents = true;
	this->PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = true;

	bRepGameplayTags = false;

	MovementReplicationSetting = EGripMovementReplicationSettings::ForceClientSideMovement;
	BreakDistance = 200.0f;

	HandleData = nullptr;
	SceneIndex = 0;

	bIsPhysicsLever = true;
	ParentComponent = nullptr;
	LeverRotationAxis = EVRInteractibleAxis::Axis_X;
	LeverLimit = 90.0f;
	LeverReturnSpeed = 50.0f;
	InitialRelativeTransform = FTransform::Identity;
	bIsLerping = false;
	bUngripAtTargetRotation = false;
	bLeverReturnsWhenReleased = true;

	// Set to only overlap with things so that its not ruined by touching over actors
	this->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

//=============================================================================
UVRLeverComponent::~UVRLeverComponent()
{
}


void UVRLeverComponent::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UVRLeverComponent, bRepGameplayTags);
	DOREPLIFETIME_CONDITION(UVRLeverComponent, GameplayTags, COND_Custom);
}

void UVRLeverComponent::PreReplication(IRepChangedPropertyTracker & ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);

	// Don't replicate if set to not do it
	DOREPLIFETIME_ACTIVE_OVERRIDE(UVRLeverComponent, GameplayTags, bRepGameplayTags);
}

void UVRLeverComponent::BeginPlay()
{
	// Call the base class 
	Super::BeginPlay();

	ResetInitialLeverLocation();
}

void UVRLeverComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	// Call supers tick (though I don't think any of the base classes to this actually implement it)
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float currentRot;
	if (ParentComponent.IsValid())
	{
		currentRot = GetAxisValue(this->GetComponentTransform().GetRelativeTransform(ParentComponent->GetComponentTransform()).GetRotation().Rotator());
	}
	else
	{
		currentRot = GetAxisValue(this->RelativeRotation);
	}

	CurrentLeverAngle = currentRot - GetAxisValue(InitialRelativeTransform.GetRotation().Rotator());

	if (bIsLerping)
	{
		// Lerp back to initial position
		float lerpToRot = GetAxisValue(InitialRelativeTransform.GetRotation().Rotator());
		float LerpedVal = FMath::FInterpConstantTo(GetAxisValue(this->RelativeRotation), lerpToRot, DeltaTime, LeverReturnSpeed);
		if (FMath::IsNearlyEqual(GetAxisValue(this->RelativeRotation), lerpToRot))
		{
			this->SetComponentTickEnabled(false);
			this->SetRelativeRotation(this->SetAxisValue(lerpToRot, this->RelativeRotation));
		}
		else
			this->SetRelativeRotation(this->SetAxisValue(LerpedVal, this->RelativeRotation));
	}
	else
	{
		if (FMath::Abs(CurrentLeverAngle) >= LeverLimit)
		{
			OnLeverStateChanged.Broadcast(true);

			if (bUngripAtTargetRotation && HoldingController)
			{
				HoldingController->DropObjectByInterface(this);
			}
		}
	}
}

void UVRLeverComponent::OnUnregister()
{
	DestroyConstraint();
	Super::OnUnregister();
}

void UVRLeverComponent::TickGrip_Implementation(UGripMotionControllerComponent * GrippingController, const FBPActorGripInformation & GripInformation, FVector MControllerLocDelta, float DeltaTime) 
{
	// Handle manual tracking here
}

void UVRLeverComponent::OnGrip_Implementation(UGripMotionControllerComponent * GrippingController, const FBPActorGripInformation & GripInformation) 
{
	ParentComponent = this->GetAttachParent();

	if (bIsPhysicsLever)
	{
		SetupConstraint();
	}

	bIsLerping = false;
	this->SetComponentTickEnabled(true);
}

void UVRLeverComponent::OnGripRelease_Implementation(UGripMotionControllerComponent * ReleasingController, const FBPActorGripInformation & GripInformation) 
{
	if(bIsPhysicsLever)
	{
		DestroyConstraint();
		FAttachmentTransformRules AttachRules(EAttachmentRule::KeepWorld, true);
		this->AttachToComponent(ParentComponent.Get(), AttachRules);
	}

	if(bLeverReturnsWhenReleased)
		bIsLerping = true;
	else
		this->SetComponentTickEnabled(false);
}

void UVRLeverComponent::OnChildGrip_Implementation(UGripMotionControllerComponent * GrippingController, const FBPActorGripInformation & GripInformation) {}
void UVRLeverComponent::OnChildGripRelease_Implementation(UGripMotionControllerComponent * ReleasingController, const FBPActorGripInformation & GripInformation) {}
void UVRLeverComponent::OnSecondaryGrip_Implementation(USceneComponent * SecondaryGripComponent, const FBPActorGripInformation & GripInformation) {}
void UVRLeverComponent::OnSecondaryGripRelease_Implementation(USceneComponent * ReleasingSecondaryGripComponent, const FBPActorGripInformation & GripInformation) {}
void UVRLeverComponent::OnUsed_Implementation() {}
void UVRLeverComponent::OnEndUsed_Implementation() {}
void UVRLeverComponent::OnSecondaryUsed_Implementation() {}
void UVRLeverComponent::OnEndSecondaryUsed_Implementation() {}

bool UVRLeverComponent::DenyGripping_Implementation()
{
	return false;//VRGripInterfaceSettings.bDenyGripping;
}

EGripInterfaceTeleportBehavior UVRLeverComponent::TeleportBehavior_Implementation()
{
	return EGripInterfaceTeleportBehavior::DropOnTeleport;
}

bool UVRLeverComponent::SimulateOnDrop_Implementation()
{
	return false;
}

EGripCollisionType UVRLeverComponent::SlotGripType_Implementation()
{
	if (bIsPhysicsLever)
		return EGripCollisionType::ManipulationGrip;
	else
		return EGripCollisionType::CustomGrip;
}

EGripCollisionType UVRLeverComponent::FreeGripType_Implementation()
{
	if (bIsPhysicsLever)
		return EGripCollisionType::ManipulationGrip;
	else
		return EGripCollisionType::CustomGrip;
}

ESecondaryGripType UVRLeverComponent::SecondaryGripType_Implementation()
{
	return ESecondaryGripType::SG_None;
}


EGripMovementReplicationSettings UVRLeverComponent::GripMovementReplicationType_Implementation()
{
	return MovementReplicationSetting;
}

EGripLateUpdateSettings UVRLeverComponent::GripLateUpdateSetting_Implementation()
{
	return EGripLateUpdateSettings::LateUpdatesAlwaysOff;
}

float UVRLeverComponent::GripStiffness_Implementation()
{
	return 1500.0f;
}

float UVRLeverComponent::GripDamping_Implementation()
{
	return 200.0f;
}

FBPAdvGripPhysicsSettings UVRLeverComponent::AdvancedPhysicsSettings_Implementation()
{
	return FBPAdvGripPhysicsSettings();
}

float UVRLeverComponent::GripBreakDistance_Implementation()
{
	return BreakDistance;
}

void UVRLeverComponent::ClosestSecondarySlotInRange_Implementation(FVector WorldLocation, bool & bHadSlotInRange, FTransform & SlotWorldTransform, FName OverridePrefix)
{
	bHadSlotInRange = false;
}

void UVRLeverComponent::ClosestPrimarySlotInRange_Implementation(FVector WorldLocation, bool & bHadSlotInRange, FTransform & SlotWorldTransform, FName OverridePrefix)
{
	bHadSlotInRange = false;
}

bool UVRLeverComponent::IsInteractible_Implementation()
{
	return false;
}

void UVRLeverComponent::IsHeld_Implementation(UGripMotionControllerComponent *& CurHoldingController, bool & bCurIsHeld)
{
	CurHoldingController = HoldingController;
	bCurIsHeld = bIsHeld;
}

void UVRLeverComponent::SetHeld_Implementation(UGripMotionControllerComponent * NewHoldingController, bool bNewIsHeld)
{
	bIsHeld = bNewIsHeld;

	if (bIsHeld)
		HoldingController = NewHoldingController;
	else
		HoldingController = nullptr;
}

FBPInteractionSettings UVRLeverComponent::GetInteractionSettings_Implementation()
{
	return FBPInteractionSettings();
}
