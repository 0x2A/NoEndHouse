// Copyright (c) 2016 Stift15 Entertainment

#pragma once

#include "GameFramework/Actor.h"
#include "InteractiveObject.generated.h"

/*UCLASS()
class NOENDHOUSE_API AObservableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObservableObject();

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Physics)
	class UPhysicsHandleComponent* PhysicsHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Observation)
	bool bObserving;
	
	UFUNCTION(BlueprintImplementableEvent, Category = Observation)
	void BeginObservation();

	UFUNCTION(BlueprintImplementableEvent, Category = Observation)
	void EndObservation();

	UFUNCTION(BlueprintImplementableEvent, Category = Observation)
	void ObservingChangedRotation(FTransform transform);
};*/


UINTERFACE(MinimalAPI)
class UInteractiveObject : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IInteractiveObject
{
	GENERATED_IINTERFACE_BODY()

	UFUNCTION(BlueprintNativeEvent)
	void BeginInteraction();

	UFUNCTION(BlueprintNativeEvent)
	void BeginObservation();

	UFUNCTION(BlueprintNativeEvent)
	void EndObservation();

	UFUNCTION(BlueprintNativeEvent)
	void ObservingChangedRotation(FRotator newRotation);

	UFUNCTION(BlueprintNativeEvent)
	bool CanPickup();

	UFUNCTION(BlueprintNativeEvent)
	bool AllowInteraction();
};