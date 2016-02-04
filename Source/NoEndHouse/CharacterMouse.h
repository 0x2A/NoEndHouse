// Copyright (c) 2016 Stift15 Entertainment

#pragma once

#include "GameFramework/Character.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	TArray<class AActor*> TargetPositions;
};
