// Copyright (c) 2016 Stift15 Entertainment

#pragma once

#include "GameFramework/Character.h"
#include "ObservableObject.h"
#include "Engine/TargetPoint.h"
#include "CharacterMouse.generated.h"

UCLASS()
class NOENDHOUSE_API ACharacterMouse : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterMouse();

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class UStaticMeshComponent* MouseMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class UAudioComponent* RatSound;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;


	UFUNCTION(BlueprintCallable, Category=AI)
	void DisableMovement();

	UFUNCTION(BlueprintCallable, Category = AI)
	void EnableMovement();


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AI)
	bool ShouldMove;

	UPROPERTY(BlueprintReadOnly, Category = AI)
	TArray<class ATargetPoint*> TargetPositions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	FString WaypointPrefix;
};
