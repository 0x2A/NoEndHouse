// Copyright (c) 2016 Stift15 Entertainment

#pragma once

#include "GameFramework/Actor.h"
#include "ObservableObject.generated.h"

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
class UObservableObject : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IObservableObject
{
	GENERATED_IINTERFACE_BODY()

	UFUNCTION(BlueprintImplementableEvent)
	void BeginObservation();

	UFUNCTION(BlueprintImplementableEvent)
	void EndObservation();

	UFUNCTION(BlueprintImplementableEvent)
	void ObservingChangedRotation(FRotator newRotation);

};