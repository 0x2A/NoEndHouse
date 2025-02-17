// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "InventoryInterface.h"
#include "InteractiveObject.h"
#include "NEHMotionController.h"
#include "NoEndHouseCharacter.generated.h"

class UInputComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBeginBlinkDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEndBlinkDelegate, float, BlinkTime);


UCLASS(config=Game)
class ANoEndHouseCharacter : public ACharacter, public IInventoryInterface
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class USkeletalMeshComponent* Mesh1P;

	UPROPERTY(EditAnywhere, Category = Mesh)
	class USkeletalMesh* HandMesh;

	UPROPERTY(EditAnywhere, Category = Mesh)
	TSubclassOf<UAnimInstance> HandAnimationBP;

	/** Location on VR gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* VROrigin;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	/** current player controller */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	class APlayerController* PlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	class UPhysicsHandleComponent* PhysicsHandleLoc;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	class UPhysicsHandleComponent* PhysicsHandleRot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UPointLightComponent* AmbientLight;

	/** Motion controller (right hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class ANEHMotionController* R_MotionController;

	/** Motion controller (left hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class ANEHMotionController* L_MotionController;

	bool bCameraShakeWalking;
	bool bCameraShakeWalkingRight;

	
	void SetCameraWalkShakeSpeed(float tSpeed);

	void HandleCameraShaking(float value, bool MoveRightLeft);

public:
	ANoEndHouseCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
		TArray<FString> Inventory;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultPlayerHeight;

	//crouching variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		FVector vCameraLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	bool bIsCrouching;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay)
	bool bUseHMD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float Sanity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	TSubclassOf<UCameraShake> CameraShakeWalk;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	TSubclassOf<UCameraShake> CameraShakeWalkCrouching;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	TSubclassOf<UCameraShake> CameraShakeSanity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Observation)
	float MaxObservationDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Observation)
	float MaxObservationMass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Observation)
	float ThrowStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool bCanMove;

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void EnableAmbientLight(bool enabled);


	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void SetSanity(float value);

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void AddSanity(float value);

	UFUNCTION(BlueprintImplementableEvent, Category = Movement)
	void OnMoveForward(float val);

	UFUNCTION(BlueprintImplementableEvent, Category = Movement)
	void OnMoveRight(float val);

	UFUNCTION(BlueprintCallable, Category = "Pawn|Character")
	virtual void Jump() override;

	UFUNCTION(BlueprintNativeEvent, Category = Movement)
	void DoCrouch();

	UFUNCTION(BlueprintNativeEvent, Category = Movement)
	void StopCrouching();

	UFUNCTION(BlueprintNativeEvent, Category = Gameplay)
		void OnBlink();

	UFUNCTION(BlueprintNativeEvent, Category = Gameplay)
		void OnEndBlinking(const float blinkTime);

	UPROPERTY(BlueprintAssignable, Category = "Gameplay")
		FBeginBlinkDelegate OnBeginBlinkDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Gameplay")
		FEndBlinkDelegate OnEndBlinkDelegate;


	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	void Tick(float DeltaSeconds) override;

	//used to set a blink material from content browser
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Gameplay)
	UMaterialInterface* BlinkMaterial;

	//Used to hold a material instance so we can modify its properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
	class UMaterialInstanceDynamic* BlinkMaterialInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Gameplay)
		float ObjectObservationRotationSpeed;


	void BeginPlay() override;

	void AddInventory(FString item) override;
	void RemoveInventory(FString item) override;
	bool CheckInventory(FString item) override;


	//Sound

	UFUNCTION(BlueprintCallable, Category = Sound)
	void PlayFootstep();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
	class USoundCue* FootstepsWood;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
	class USoundCue* FootstepsConcrete;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
	class USoundCue* FootstepsDirt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
	class USoundCue* FootstepsGravel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
	class USoundCue* FootstepsTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
	class USoundCue* FootstepsWater;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
	float FootstepSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
	float FootstepSpeedCrouched;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float MaxZoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float BaseFOV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float ZoomSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool SoundEnabled;

	UPROPERTY(BlueprintReadOnly, Category = Gameplay)
		bool bZooming;

	bool bFootstepSoundPlaying;
	bool bWasCrouching;
	
	/* Handle to manage the timer */
	FTimerHandle FootstepTimerHandle;

	void StartPlayFootsteps(float Value);
	void StopPlayFootsteps();


	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void StopObservingObject(bool applyDefaultPhysics = true);

	void OnSaveGame(class UNEHSaveGame* savegame);

protected:

	void Blink();
	void StopBlinking();

	/** Called when this Pawn is possessed. Only called on the server (or in standalone). */
	void PossessedBy(AController* NewController) override;

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	void SetObservationDistance(float val);

	void Use();

	void EndUse();

	void BeginZoom();
	void EndZoom();

	void BeginRotateObservedObject();
	void EndRotateObservedObject();

	void HideGrabIcon();
	void ShowGrabIcon();


	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	struct TouchData
	{
		TouchData() { bIsPressed = false;Location=FVector::ZeroVector;}
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;

	float fBeginBlinkSeconds;
	float fBlinkSeconds;
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/* 
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so 
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);

	void BeginObjectInteraction();
	void EndObjectInteraction();

	void BeginGrabLeft();
	void EndGrabLeft();
	void BeginGrabRight();
	void EndGrabRight();

	void StartObserving();
	void EndObserving(bool applyForces);

	UFUNCTION()
	void EndObservingDestructable(const FVector& HitPoint, const FVector& HitDirection);

	bool bPhysicsHandleActive;
	bool bIsHeld;
	bool bRotateHeldObject;
	FRotator obsObjRotationOffset;

	TWeakObjectPtr<class AActor> HitResultObservObject;
	TWeakObjectPtr<class UPrimitiveComponent> HitResultObservComponent;

	UStaticMeshComponent* lastStaticMeshComp;

	float ObservingObjectDistance;

	float fLastTime;
	float fCurrentTime;
public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

