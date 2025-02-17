// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "GrippableBoxComponent.h"

//=============================================================================
UGrippableBoxComponent::~UGrippableBoxComponent()
{
}

  //=============================================================================
UGrippableBoxComponent::UGrippableBoxComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	VRGripInterfaceSettings.bDenyGripping = false;
	VRGripInterfaceSettings.OnTeleportBehavior = EGripInterfaceTeleportBehavior::DropOnTeleport;
	VRGripInterfaceSettings.bSimulateOnDrop = true;
	VRGripInterfaceSettings.SlotDefaultGripType = EGripCollisionType::ManipulationGrip;
	VRGripInterfaceSettings.FreeDefaultGripType = EGripCollisionType::ManipulationGrip;
	//VRGripInterfaceSettings.bCanHaveDoubleGrip = false;
	VRGripInterfaceSettings.SecondaryGripType = ESecondaryGripType::SG_None;
	//VRGripInterfaceSettings.GripTarget = EGripTargetType::ComponentGrip;
	VRGripInterfaceSettings.MovementReplicationType = EGripMovementReplicationSettings::ForceClientSideMovement;
	VRGripInterfaceSettings.LateUpdateSetting = EGripLateUpdateSettings::LateUpdatesAlwaysOff;
	VRGripInterfaceSettings.ConstraintStiffness = 1500.0f;
	VRGripInterfaceSettings.ConstraintDamping = 200.0f;
	VRGripInterfaceSettings.ConstraintBreakDistance = 100.0f;
	VRGripInterfaceSettings.SecondarySlotRange = 20.0f;
	VRGripInterfaceSettings.PrimarySlotRange = 20.0f;
	VRGripInterfaceSettings.bIsInteractible = false;
	VRGripInterfaceSettings.bIsHeld = false;
	VRGripInterfaceSettings.HoldingController = nullptr;

	this->bReplicates = true;

	bRepGripSettingsAndGameplayTags = true;
}

void UGrippableBoxComponent::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	 Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGrippableBoxComponent, bRepGripSettingsAndGameplayTags);
	DOREPLIFETIME_CONDITION(UGrippableBoxComponent, VRGripInterfaceSettings, COND_Custom);
	DOREPLIFETIME_CONDITION(UGrippableBoxComponent, GameplayTags, COND_Custom);
}

void UGrippableBoxComponent::PreReplication(IRepChangedPropertyTracker & ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);

	// Don't replicate if set to not do it
	DOREPLIFETIME_ACTIVE_OVERRIDE(UGrippableBoxComponent, VRGripInterfaceSettings, bRepGripSettingsAndGameplayTags);
	DOREPLIFETIME_ACTIVE_OVERRIDE(UGrippableBoxComponent, GameplayTags, bRepGripSettingsAndGameplayTags);
}

void UGrippableBoxComponent::TickGrip_Implementation(UGripMotionControllerComponent * GrippingController, const FBPActorGripInformation & GripInformation, FVector MControllerLocDelta, float DeltaTime) {}
void UGrippableBoxComponent::OnGrip_Implementation(UGripMotionControllerComponent * GrippingController, const FBPActorGripInformation & GripInformation) {}
void UGrippableBoxComponent::OnGripRelease_Implementation(UGripMotionControllerComponent * ReleasingController, const FBPActorGripInformation & GripInformation) {}
void UGrippableBoxComponent::OnChildGrip_Implementation(UGripMotionControllerComponent * GrippingController, const FBPActorGripInformation & GripInformation) {}
void UGrippableBoxComponent::OnChildGripRelease_Implementation(UGripMotionControllerComponent * ReleasingController, const FBPActorGripInformation & GripInformation) {}
void UGrippableBoxComponent::OnSecondaryGrip_Implementation(USceneComponent * SecondaryGripComponent, const FBPActorGripInformation & GripInformation) {}
void UGrippableBoxComponent::OnSecondaryGripRelease_Implementation(USceneComponent * ReleasingSecondaryGripComponent, const FBPActorGripInformation & GripInformation) {}
void UGrippableBoxComponent::OnUsed_Implementation() {}
void UGrippableBoxComponent::OnEndUsed_Implementation() {}
void UGrippableBoxComponent::OnSecondaryUsed_Implementation() {}
void UGrippableBoxComponent::OnEndSecondaryUsed_Implementation() {}

bool UGrippableBoxComponent::DenyGripping_Implementation()
{
	return VRGripInterfaceSettings.bDenyGripping;
}

EGripInterfaceTeleportBehavior UGrippableBoxComponent::TeleportBehavior_Implementation()
{
	return VRGripInterfaceSettings.OnTeleportBehavior;
}

bool UGrippableBoxComponent::SimulateOnDrop_Implementation()
{
	return VRGripInterfaceSettings.bSimulateOnDrop;
}

EGripCollisionType UGrippableBoxComponent::SlotGripType_Implementation()
{
	return VRGripInterfaceSettings.SlotDefaultGripType;
}

EGripCollisionType UGrippableBoxComponent::FreeGripType_Implementation()
{
	return VRGripInterfaceSettings.FreeDefaultGripType;
}

/*bool UGrippableBoxComponent::CanHaveDoubleGrip_Implementation()
{
	return VRGripInterfaceSettings.bCanHaveDoubleGrip;
}*/

ESecondaryGripType UGrippableBoxComponent::SecondaryGripType_Implementation()
{
	return VRGripInterfaceSettings.SecondaryGripType;
}


/*EGripTargetType UGrippableBoxComponent::GripTargetType_Implementation()
{
	return VRGripInterfaceSettings.GripTarget;
}*/

EGripMovementReplicationSettings UGrippableBoxComponent::GripMovementReplicationType_Implementation()
{
	return VRGripInterfaceSettings.MovementReplicationType;
}

EGripLateUpdateSettings UGrippableBoxComponent::GripLateUpdateSetting_Implementation()
{
	return VRGripInterfaceSettings.LateUpdateSetting;
}

float UGrippableBoxComponent::GripStiffness_Implementation()
{
	return VRGripInterfaceSettings.ConstraintStiffness;
}

float UGrippableBoxComponent::GripDamping_Implementation()
{
	return VRGripInterfaceSettings.ConstraintDamping;
}

FBPAdvGripPhysicsSettings UGrippableBoxComponent::AdvancedPhysicsSettings_Implementation()
{
	return VRGripInterfaceSettings.AdvancedPhysicsSettings;
}

float UGrippableBoxComponent::GripBreakDistance_Implementation()
{
	return VRGripInterfaceSettings.ConstraintBreakDistance;
}

void UGrippableBoxComponent::ClosestSecondarySlotInRange_Implementation(FVector WorldLocation, bool & bHadSlotInRange, FTransform & SlotWorldTransform, FName OverridePrefix)
{
	if (OverridePrefix.IsNone())
		OverridePrefix = "VRGripS";

	UVRExpansionFunctionLibrary::GetGripSlotInRangeByTypeName_Component(OverridePrefix, this, WorldLocation, VRGripInterfaceSettings.SecondarySlotRange, bHadSlotInRange, SlotWorldTransform);
}

void UGrippableBoxComponent::ClosestPrimarySlotInRange_Implementation(FVector WorldLocation, bool & bHadSlotInRange, FTransform & SlotWorldTransform, FName OverridePrefix)
{
	if (OverridePrefix.IsNone())
		OverridePrefix = "VRGripP";

	UVRExpansionFunctionLibrary::GetGripSlotInRangeByTypeName_Component(OverridePrefix, this, WorldLocation, VRGripInterfaceSettings.PrimarySlotRange, bHadSlotInRange, SlotWorldTransform);
}

bool UGrippableBoxComponent::IsInteractible_Implementation()
{
	return VRGripInterfaceSettings.bIsInteractible;
}

void UGrippableBoxComponent::IsHeld_Implementation(UGripMotionControllerComponent *& HoldingController, bool & bIsHeld)
{
	HoldingController = VRGripInterfaceSettings.HoldingController;
	bIsHeld = VRGripInterfaceSettings.bIsHeld;
}

void UGrippableBoxComponent::SetHeld_Implementation(UGripMotionControllerComponent * HoldingController, bool bIsHeld)
{
	if (bIsHeld)
		VRGripInterfaceSettings.HoldingController = HoldingController;
	else
		VRGripInterfaceSettings.HoldingController = nullptr;

	VRGripInterfaceSettings.bIsHeld = bIsHeld;
}

FBPInteractionSettings UGrippableBoxComponent::GetInteractionSettings_Implementation()
{
	return VRGripInterfaceSettings.InteractionSettings;
}
