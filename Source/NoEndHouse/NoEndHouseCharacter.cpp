// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "NoEndHouse.h"
#include "NoEndHouseCharacter.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/InputSettings.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "PhysicsEngine/DestructibleActor.h"
#include "Components/DestructibleComponent.h"

#include "Sound/SoundCue.h"
#include "NEHGameState.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ANoEndHouseCharacter

ANoEndHouseCharacter::ANoEndHouseCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->AttachParent = GetCapsuleComponent();
	FirstPersonCameraComponent->RelativeLocation = FVector(0, 0, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->AttachParent = FirstPersonCameraComponent;
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;

	bCameraShakeWalking = bCameraShakeWalkingRight = false;

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	//chrouching
	vCameraLocation = FirstPersonCameraComponent->RelativeLocation;
	bIsCrouching = bWasCrouching = false;

	FootstepSpeed = 0.47f;
	FootstepSpeedCrouched = 0.65f;
	bFootstepSoundPlaying = false;

	MaxObservationDistance = 180;
	MaxObservationMass = 100;
	bRotateHeldObject = false;
	ObjectObservationRotationSpeed = 12.0f;
	ThrowStrength = 500.0f;
	// *** This seems to be still broken in 4.9.2 ***
	//Initialize camera shaking class
	/*CameraShake = ConstructObject<UCameraShake>(UCameraShake::StaticClass());
	CameraShake->OscillationDuration = 1.0f; //negative value will run forever
	CameraShake->OscillationBlendInTime = 0.1f;
	CameraShake->OscillationBlendOutTime = 0.2f;

	CameraShake->RotOscillation.Pitch.Amplitude = 1.0f;
	CameraShake->RotOscillation.Pitch.Frequency = 20.0f;
	CameraShake->RotOscillation.Pitch.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;

	CameraShake->RotOscillation.Yaw.Amplitude = 0.0f;
	CameraShake->RotOscillation.Yaw.Frequency = 5.0f;
	CameraShake->RotOscillation.Yaw.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;

	CameraShake->RotOscillation.Roll.Amplitude = 1.0f;
	CameraShake->RotOscillation.Roll.Frequency = 9.0f;<
	CameraShake->RotOscillation.Roll.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;

	CameraShake->LocOscillation.Y.Amplitude = 0.0f;
	CameraShake->LocOscillation.Z.Amplitude = 2.0f;
	CameraShake->LocOscillation.Z.Frequency = 20.0f;
	CameraShake->LocOscillation.Z.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;*/

	bPhysicsHandleActive = false;
	PhysicsHandleLoc = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandleLoc"));
	PhysicsHandleLoc->AngularDamping = 0.0f;
	PhysicsHandleLoc->AngularStiffness = 0.0f;
	PhysicsHandleLoc->InterpolationSpeed = 100.0f;

	PhysicsHandleRot = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandleRot"));
	PhysicsHandleRot->LinearDamping = 0.0f;
	PhysicsHandleRot->LinearStiffness = 0.0f;
	PhysicsHandleRot->AngularDamping = 500.0f;
	PhysicsHandleRot->AngularStiffness = 1000.0f;
	PhysicsHandleRot->InterpolationSpeed = 100.0f;

	PlayerController = nullptr;
	
	MaxZoom = 55.0f;
	bZooming = false;
	ZoomSpeed = 4.0f;
	BaseFOV = 90.0f;

	Sanity = 100.0f;

	FirstPersonCameraComponent->PostProcessSettings.bOverride_SceneFringeIntensity = true;
	FirstPersonCameraComponent->PostProcessSettings.bOverride_VignetteIntensity = true;


}

//////////////////////////////////////////////////////////////////////////
// Input

void ANoEndHouseCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAction("Crouch", IE_Pressed, this, &ANoEndHouseCharacter::Crouch);
	InputComponent->BindAction("Crouch", IE_Released, this, &ANoEndHouseCharacter::StopCrouching);

	InputComponent->BindAction("Blink", IE_Pressed, this, &ANoEndHouseCharacter::Blink);
	InputComponent->BindAction("Blink", IE_Released, this, &ANoEndHouseCharacter::StopBlinking);

	InputComponent->BindAction("Use", IE_Pressed, this, &ANoEndHouseCharacter::Use);
	InputComponent->BindAction("Use", IE_Released, this, &ANoEndHouseCharacter::EndUse);

	InputComponent->BindAction("ObjectInteraction", IE_Pressed, this, &ANoEndHouseCharacter::BeginObjectInteraction);
	InputComponent->BindAction("ObjectInteraction", IE_Released, this, &ANoEndHouseCharacter::EndObjectInteraction);
	
	InputComponent->BindAction("Zoom", IE_Pressed, this, &ANoEndHouseCharacter::BeginZoom);
	InputComponent->BindAction("Zoom", IE_Released, this, &ANoEndHouseCharacter::EndZoom);

	InputComponent->BindAxis("MoveForward", this, &ANoEndHouseCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ANoEndHouseCharacter::MoveRight);

	InputComponent->BindAxis("ObservationDistance", this, &ANoEndHouseCharacter::SetObservationDistance);
	
	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &ANoEndHouseCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &ANoEndHouseCharacter::LookUpAtRate);
}

void ANoEndHouseCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if( TouchItem.bIsPressed == true )
	{
		return;
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void ANoEndHouseCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

void ANoEndHouseCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if ((TouchItem.bIsPressed == true) && ( TouchItem.FingerIndex==FingerIndex))
	{
		if (TouchItem.bIsPressed)
		{
			if (GetWorld() != nullptr)
			{
				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
				if (ViewportClient != nullptr)
				{
					FVector MoveDelta = Location - TouchItem.Location;
					FVector2D ScreenSize;
					ViewportClient->GetViewportSize(ScreenSize);
					FVector2D ScaledDelta = FVector2D( MoveDelta.X, MoveDelta.Y) / ScreenSize;									
					if (ScaledDelta.X != 0.0f)
					{
						TouchItem.bMoved = true;
						float Value = ScaledDelta.X * BaseTurnRate;
						AddControllerYawInput(Value);
					}
					if (ScaledDelta.Y != 0.0f)
					{
						TouchItem.bMoved = true;
						float Value = ScaledDelta.Y* BaseTurnRate;
						AddControllerPitchInput(Value);
					}
					TouchItem.Location = Location;
				}
				TouchItem.Location = Location;
			}
		}
	}
}

void ANoEndHouseCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		if (GetCharacterMovement()->IsMovingOnGround() && GetCharacterMovement()->IsWalking())
			StartPlayFootsteps();

		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);

		//Handle camera shake
		if (PlayerController)
		{
			if (GetCharacterMovement()->IsWalking())
			{
				if (!bCameraShakeWalking)
				{
					bCameraShakeWalking = true;
					if (!bCameraShakeWalkingRight)
						PlayerController->ClientPlayCameraShake(CameraShakeWalk, fabs(Value));
				}
			}
			else
				bCameraShakeWalking = false;
		}
	}
	else
	{
		bCameraShakeWalking = false;
	}

	//only stop if both not enabled
	if (PlayerController && !bCameraShakeWalking && !bCameraShakeWalkingRight)
		PlayerController->ClientStopCameraShake(CameraShakeWalk);

	OnMoveForward(Value);

	if (GetCharacterMovement()->Velocity.IsZero() || !GetCharacterMovement()->IsMovingOnGround()) StopPlayFootsteps();
}

void ANoEndHouseCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		if (GetCharacterMovement()->IsMovingOnGround() && GetCharacterMovement()->IsWalking())
			StartPlayFootsteps();

		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);

		//Handle camera shake
		if (PlayerController)
		{
			if (GetCharacterMovement()->IsWalking())
			{
				if (!bCameraShakeWalkingRight)
				{
					bCameraShakeWalkingRight = true;
					if (!bCameraShakeWalking)
						PlayerController->ClientPlayCameraShake(CameraShakeWalk, fabs(Value));
				}
			}
			else
				bCameraShakeWalkingRight = false;
		}
	}
	else
	{
		bCameraShakeWalkingRight = false;
	}

	OnMoveRight(Value);
	if (GetCharacterMovement()->Velocity.IsZero() || !GetCharacterMovement()->IsMovingOnGround()) StopPlayFootsteps();
}


void ANoEndHouseCharacter::Crouch_Implementation()
{
	bIsCrouching = true;
	ACharacter::Crouch();
}

void ANoEndHouseCharacter::StopCrouching_Implementation()
{
	bIsCrouching = false;
	ACharacter::UnCrouch();
}


void ANoEndHouseCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ANoEndHouseCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	
}

bool ANoEndHouseCharacter::EnableTouchscreenMovement(class UInputComponent* InputComponent)
{
	bool bResult = false;
	if(FPlatformMisc::GetUseVirtualJoysticks() || GetDefault<UInputSettings>()->bUseMouseForTouch )
	{
		bResult = true;
		InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ANoEndHouseCharacter::BeginTouch);
		InputComponent->BindTouch(EInputEvent::IE_Released, this, &ANoEndHouseCharacter::EndTouch);
		InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ANoEndHouseCharacter::TouchUpdate);
	}
	
	
	return bResult;
}

void ANoEndHouseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//cache current PlayerController so we dont have to cast every frame
	APlayerController *controller = Cast<APlayerController>(NewController);
	if (controller)
	{
		PlayerController = controller;
	}
}

void ANoEndHouseCharacter::OnBlink_Implementation()
{
	if (!BlinkMaterialInstance) return;
	BlinkMaterialInstance->SetScalarParameterValue("Value", 0.0f);
}

void ANoEndHouseCharacter::OnEndBlinking_Implementation(const float blinkTime)
{
	if (!BlinkMaterialInstance) return;
	BlinkMaterialInstance->SetScalarParameterValue("Value", 1.0f);

}

void ANoEndHouseCharacter::BeginPlay()
{
	Super::BeginPlay();

	//create blink material instance from blink material defined in default properties
	if (BlinkMaterial)
	{
		//first create material instance
		BlinkMaterialInstance = UMaterialInstanceDynamic::Create(BlinkMaterial, this);

		//add this instance to postProcessing settings
		FirstPersonCameraComponent->PostProcessSettings.AddBlendable(BlinkMaterialInstance, 1.0f);
	}

	//since the player is only created after loading/creating a new game or dying, we can just always
	//use the savegame if any available
	auto gameState = Cast<ANEHGameState>(GetWorld()->GetGameState());
	if (gameState && gameState->currentSaveGame)
	{
		SetActorLocation(gameState->currentSaveGame->aPlayerLocation);
		Inventory = gameState->currentSaveGame->inventory;
	}
}

void ANoEndHouseCharacter::SetSanity(float value)
{
	Sanity = FMath::Clamp(value, 0.0f, 100.0f);
	FirstPersonCameraComponent->PostProcessSettings.SceneFringeIntensity = ((100.0f - Sanity) / 100.0f) * 5.0f;
	FirstPersonCameraComponent->PostProcessSettings.VignetteIntensity = ((100.0f - Sanity) / 100.0f) * 2.0f;
	
}

void ANoEndHouseCharacter::AddInventory(FString item)
{
	Inventory.Add(item);
}

void ANoEndHouseCharacter::RemoveInventory(FString item)
{
	if (CheckInventory(item))
		Inventory.Remove(item);
}

bool ANoEndHouseCharacter::CheckInventory(FString item)
{
	return Inventory.Contains(item);
}

void ANoEndHouseCharacter::PlayFootstep()
{
	FVector location = GetActorLocation();

	//Re-initialize hit info
	FHitResult RV_Hit(ForceInit);
	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("surface_trace")), true, this);
	RV_TraceParams.bReturnPhysicalMaterial = true;
	if (GetWorld()->LineTraceSingleByChannel(RV_Hit, location, location + FVector(0, 0, -500), ECC_Visibility, RV_TraceParams))
	{
		if (RV_Hit.PhysMaterial.IsValid())
		{
			//lets default to wood footsteps
			USoundCue* cue = FootstepsWood;
			switch (RV_Hit.PhysMaterial->SurfaceType)
			{
			case SurfaceType1:
				cue = FootstepsWood;
				break;
			case SurfaceType2:
				cue = FootstepsConcrete;
				break;
			case SurfaceType3:
				cue = FootstepsDirt;
				break;
			case SurfaceType4:
				cue = FootstepsGravel;
				break;
			case SurfaceType5:
				cue = FootstepsTiles;
				break;
			case SurfaceType6:
				cue = FootstepsWater;
				break;
			}
			//Play sound cue at hit location
			if (cue) UGameplayStatics::PlaySoundAtLocation(this, cue, RV_Hit.Location);
		}
	}
}


//TODO: Get rid of this and use animation events instead
void ANoEndHouseCharacter::StartPlayFootsteps()
{
	if (bIsCrouching != bWasCrouching)
	{
		bWasCrouching = bIsCrouching;
		if (bFootstepSoundPlaying)
			StopPlayFootsteps();
	}
	if (!bFootstepSoundPlaying)
		GetWorld()->GetTimerManager().SetTimer(FootstepTimerHandle, this, &ANoEndHouseCharacter::PlayFootstep, bIsCrouching ? FootstepSpeedCrouched : FootstepSpeed, true, 0.1f);
	bFootstepSoundPlaying = true;
}

void ANoEndHouseCharacter::StopPlayFootsteps()
{
	bFootstepSoundPlaying = false;
	GetWorld()->GetTimerManager().ClearTimer(FootstepTimerHandle);
}

void ANoEndHouseCharacter::Use()
{
	if (bPhysicsHandleActive)
		EndObserving();
	else
		StartObserving();
}

void ANoEndHouseCharacter::EndUse()
{
	//EndObserving();
}

void ANoEndHouseCharacter::StartObserving()
{
	FHitResult hitResult;
	FVector camLocation = FirstPersonCameraComponent->GetComponentLocation();
	FVector targetLoc = FirstPersonCameraComponent->GetForwardVector() * MaxObservationDistance;

	if (GetWorld()->LineTraceSingleByChannel(hitResult, camLocation, camLocation + targetLoc, COLLISION_OBSERVABLE,
		FCollisionQueryParams("ObserveTrace", false, this),FCollisionResponseParams(ECR_Block)))
	{

		HitResultObservObject = hitResult.Actor;
		if (!HitResultObservObject.IsValid() || !(HitResultObservObject->GetClass()->ImplementsInterface(UObservableObject::StaticClass()) || HitResultObservObject->Tags.Contains("Observable")))
		{
			HitResultObservObject.Reset();
			return;
		}

		HitResultObservComponent = hitResult.Component;

		ObservingObjectDistance = (GetActorLocation() - hitResult.Location).Size();
		if (HitResultObservComponent->GetMass() < MaxObservationMass)
		{

			bPhysicsHandleActive = true;
			bZooming = false;
			obsObjRotationOffset = FRotator();


			FVector centerOfMass = HitResultObservComponent->GetCenterOfMass();
			FVector grabOffset = HitResultObservComponent->GetCenterOfMass() - hitResult.ImpactPoint;

			//setting location and rotation of same handle causes object to fly away at high speed rotations...
			PhysicsHandleLoc->GrabComponent(HitResultObservComponent.Get(), NAME_None, centerOfMass, false);
			//hint: the last bool is important, it prevents the object from jiggle around!
			//This cost us hours to figure out how to prevent the jiggle -.-
			PhysicsHandleRot->GrabComponent(HitResultObservComponent.Get(), NAME_None, centerOfMass, true);

			//dont collide with ourself and other pawns
			HitResultObservComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

			//we dont want to turn the object itself down again, because of its mass point, we want to rotate it by ourselves
			HitResultObservComponent->SetEnableGravity(false);

			UDestructibleComponent* destructable = Cast<UDestructibleComponent>(HitResultObservComponent.Get());
			if (destructable)
			{
				destructable->OnComponentFracture.AddDynamic(this, &ANoEndHouseCharacter::EndObservingDestructable);
			}
			if(HitResultObservObject->GetClass()->ImplementsInterface(UObservableObject::StaticClass()))
				IObservableObject::Execute_BeginObservation(HitResultObservObject.Get());
		}

	}
	//else
		//HitResultObservObject.Reset();
}

void ANoEndHouseCharacter::EndObserving()
{
	if (!bPhysicsHandleActive) return;
	bPhysicsHandleActive = false;
	
	PhysicsHandleLoc->ReleaseComponent();
	PhysicsHandleRot->ReleaseComponent();

	if (HitResultObservComponent.IsValid())
	{
		HitResultObservComponent->SetEnableGravity(true);
		UDestructibleComponent* destructable = Cast<UDestructibleComponent>(HitResultObservComponent.Get());
		if (!destructable)
		{
			HitResultObservComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
			HitResultObservComponent->WakeAllRigidBodies();

			if (bIsHeld)
			{
				FVector camLocation = FirstPersonCameraComponent->GetForwardVector();
				camLocation *= HitResultObservComponent->GetMass() * ThrowStrength;
				HitResultObservComponent->AddImpulse(camLocation);
			}
		}
	}
	if (HitResultObservObject.IsValid() && HitResultObservObject->GetClass()->ImplementsInterface(UObservableObject::StaticClass()))
		IObservableObject::Execute_EndObservation(HitResultObservObject.Get());

	EndRotateObservedObject();
	HitResultObservObject.Reset();
}

void ANoEndHouseCharacter::EndObservingDestructable(const FVector& HitPoint, const FVector& HitDirection)
{
	if (HitResultObservComponent.IsValid())
	{
		HitResultObservComponent->SetEnableGravity(true);
	}
	EndObserving();
}

void ANoEndHouseCharacter::BeginObjectInteraction()
{
	if (bPhysicsHandleActive && !bRotateHeldObject)
	{
		bIsHeld = true;
		EndObserving();
	}
}

void ANoEndHouseCharacter::EndObjectInteraction()
{
	bIsHeld = false;
}

void ANoEndHouseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bPhysicsHandleActive)
	{
		if (HitResultObservObject.IsValid())
		{
			FVector forwardRot = FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::X);
			forwardRot *= ObservingObjectDistance;
			FVector handleLocation = FirstPersonCameraComponent->GetComponentLocation() + forwardRot;

			if (bRotateHeldObject)
			{
				float dX, dY;
				
				//rotate the object depending on view direction, not local direction
				PlayerController->GetInputMouseDelta(dX, dY);
				FQuat AQuat = FQuat(obsObjRotationOffset);
				FQuat BQuat = FQuat(FRotator(-dY*ObjectObservationRotationSpeed,-dX*ObjectObservationRotationSpeed,0));
				obsObjRotationOffset = FRotator(BQuat*AQuat);
				if (HitResultObservObject->GetClass()->ImplementsInterface(UObservableObject::StaticClass()))
					IObservableObject::Execute_ObservingChangedRotation(HitResultObservObject.Get(), obsObjRotationOffset);
				
			}
			PhysicsHandleLoc->SetTargetLocation(handleLocation);
			PhysicsHandleRot->SetTargetRotation(GetControlRotation() + obsObjRotationOffset);
		}
		if (HitResultObservObject.IsValid())
		{
			if (MaxObservationDistance < (GetMesh()->GetComponentLocation() - HitResultObservComponent->GetComponentLocation()).Size())
				EndObserving();
		}
	}
	if (bZooming)
	{
		FirstPersonCameraComponent->FieldOfView = FMath::Lerp(FirstPersonCameraComponent->FieldOfView, MaxZoom, ZoomSpeed * DeltaSeconds);
	}
	else
	{
		FirstPersonCameraComponent->FieldOfView = FMath::Lerp(FirstPersonCameraComponent->FieldOfView, BaseFOV, ZoomSpeed * DeltaSeconds);
	}
}

void ANoEndHouseCharacter::SetObservationDistance(float val)
{
	ObservingObjectDistance -= val*5.0f;
	ObservingObjectDistance = FMath::Clamp(ObservingObjectDistance, 25.0f, MaxObservationDistance - 10.0f);
}

void ANoEndHouseCharacter::Blink()
{
	fBeginBlinkSeconds = GetWorld()->GetTimeSeconds();
	OnBlink();
}

void ANoEndHouseCharacter::StopBlinking()
{
	fBlinkSeconds = GetWorld()->GetTimeSeconds() - fBeginBlinkSeconds;
	OnEndBlinking(fBlinkSeconds);
}

void ANoEndHouseCharacter::BeginZoom()
{
	if (bPhysicsHandleActive)
	{
		BeginRotateObservedObject();
		return;
	}

	bZooming = true;
}

void ANoEndHouseCharacter::BeginRotateObservedObject()
{
	bRotateHeldObject = true;

	//disable camera rotation, so that we can rotate the observed object with the mouse
	PlayerController->InputPitchScale = 0;
	PlayerController->InputYawScale = 0;
}

void ANoEndHouseCharacter::EndZoom()
{
	if (bPhysicsHandleActive)
	{
		EndRotateObservedObject();
		return;
	}

	bZooming = false;
}

void ANoEndHouseCharacter::EndRotateObservedObject()
{
	bRotateHeldObject = false;

	//re-enable camera rotation
	PlayerController->InputPitchScale = -1.75f; //seems to be the default values
	PlayerController->InputYawScale = 2.5f;
}

void ANoEndHouseCharacter::OnSaveGame(class UNEHSaveGame* savegame)
{
	savegame->aPlayerLocation = GetActorLocation();
	savegame->inventory = Inventory;
	savegame->Sanity = Sanity;
}

