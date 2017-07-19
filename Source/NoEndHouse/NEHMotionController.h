// Copyright (c) 2016 Stift15 Entertainment

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NEHMotionController.generated.h"

UENUM(BlueprintType)
enum class GripType : uint8
{
	GT_Open UMETA(DisplayName = "Open"),
	GT_CanGrab UMETA(DisplayName = "CanGrab"),
	GT_Grab UMETA(DisplayName = "Grab")
};

UCLASS()
class NOENDHOUSE_API ANEHMotionController : public AActor
{
	GENERATED_BODY()
	
public:	


	// Sets default values for this actor's properties
	ANEHMotionController();

	/** Motion controller (right hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UMotionControllerComponent* MotionController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* HandMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* GrabSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USteamVRChaperoneComponent* SteamVRChaperone;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void UpdateHandAnimation();

	class AActor* GetActorNearHand();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class AActor* AttachedActor;

	void OnGrabSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = Grabbing)
	void GrabActor();

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void SetHand(EControllerHand hand, USkeletalMesh* mesh, TSubclassOf<UAnimInstance> anim);

	UFUNCTION(BlueprintCallable, Category = Grabbing)
	void ReleaseActor();

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void RumbleController(float scale = 1.0f);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EControllerHand Hand;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bWantsToGrip;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bAlwaysCollideHands;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		GripType GripState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UHapticFeedbackEffect_Base* HapticEffect;
};
