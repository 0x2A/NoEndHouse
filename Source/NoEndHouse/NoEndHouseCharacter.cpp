// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "NoEndHouse.h"
#include "NoEndHouseCharacter.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/InputSettings.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Sound/SoundCue.h"

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
	CameraShake->RotOscillation.Roll.Frequency = 9.0f;
	CameraShake->RotOscillation.Roll.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;

	CameraShake->LocOscillation.Y.Amplitude = 0.0f;
	CameraShake->LocOscillation.Z.Amplitude = 2.0f;
	CameraShake->LocOscillation.Z.Frequency = 20.0f;
	CameraShake->LocOscillation.Z.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;*/

	PlayerController = nullptr;
	
	Sanity = 100.0f;
	FirstPersonCameraComponent->PostProcessSettings.bOverride_SceneFringeIntensity = true;
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

	InputComponent->BindAxis("MoveForward", this, &ANoEndHouseCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ANoEndHouseCharacter::MoveRight);
	
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
						PlayerController->ClientPlayCameraShake(CameraShakeWalk, abs(Value));
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
						PlayerController->ClientPlayCameraShake(CameraShakeWalk, abs(Value));
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

void ANoEndHouseCharacter::Blink_Implementation()
{
	if (!BlinkMaterialInstance) return;
	BlinkMaterialInstance->SetScalarParameterValue("Value", 0.0f);
}

void ANoEndHouseCharacter::StopBlinking_Implementation()
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
}

void ANoEndHouseCharacter::SetSanity(float value)
{
	Sanity = FMath::Clamp(value, 0.0f, 100.0f);
	FirstPersonCameraComponent->PostProcessSettings.SceneFringeIntensity = ((100.0f - Sanity) / 100.0f) * 5.0f;
	
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

