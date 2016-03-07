// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "InventoryInterface.h"
#include "ObservableObject.h"
#include "NoEndHouseCharacter.generated.h"

class UInputComponent;

UCLASS(config=Game)
class ANoEndHouseCharacter : public ACharacter, public IInventoryInterface
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class USkeletalMeshComponent* Mesh1P;


	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	/** current player controller */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	class APlayerController* PlayerController;



	bool bCameraShakeWalking;
	bool bCameraShakeWalkingRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	TArray<FString> Inventory;

public:
	ANoEndHouseCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	//crouching variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		FVector vCameraLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	bool bIsCrouching;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float Sanity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	TSubclassOf<UCameraShake> CameraShakeWalk;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Observation)
	float MaxObservationDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Observation)
	float MaxObservationMass;

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void SetSanity(float value);

	UFUNCTION(BlueprintImplementableEvent, Category = Movement)
	void OnMoveForward(float val);

	UFUNCTION(BlueprintImplementableEvent, Category = Movement)
	void OnMoveRight(float val);

	UFUNCTION(BlueprintNativeEvent, Category = Movement)
	void Crouch();

	UFUNCTION(BlueprintNativeEvent, Category = Movement)
	void StopCrouching();



	UFUNCTION(BlueprintNativeEvent, Category = Movement)
		void OnBlink();

	UFUNCTION(BlueprintNativeEvent, Category = Movement)
		void OnEndBlinking(const float blinkTime);

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

	bool bFootstepSoundPlaying;
	bool bWasCrouching;
	
	/* Handle to manage the timer */
	FTimerHandle FootstepTimerHandle;

	void StartPlayFootsteps();
	void StopPlayFootsteps();

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

	void StartObserving();
	void EndObserving();

	bool bPhysicsHandleActive;
	bool bIsHeld;
	bool bRotateHeldObject;
	FRotator obsObjRotationOffset;

	TWeakObjectPtr<class AActor> HitResultObservObject;
	TWeakObjectPtr<class UPrimitiveComponent> HitResultObservComponent;
	UPhysicsHandleComponent* HitResultObservPhysHandle;
	float ObservingObjectDistance;

public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

