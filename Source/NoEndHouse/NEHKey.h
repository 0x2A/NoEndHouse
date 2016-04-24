// Copyright (c) 2016 Stift15 Entertainment

#pragma once

#include "GameFramework/Actor.h"
#include "NEHKey.generated.h"

UCLASS()
class NOENDHOUSE_API ANEHKey : public AActor
{
	GENERATED_BODY()
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* KeyMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FX, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* DissolveParticleEffect;

public:	
	// Sets default values for this actor's properties
	ANEHKey();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, category=Gameplay)
	void PickUp();

	UFUNCTION(BlueprintImplementableEvent, Category = Gameplay)
	void OnPickUp();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FString KeyID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool bPickedUp;


	UPROPERTY(BlueprintReadOnly, Category = Gameplay)
		TArray<class UMaterialInstanceDynamic*> MaterialInstances;
};
