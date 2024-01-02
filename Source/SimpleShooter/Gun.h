// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

	void PullTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Root;

	// Mesh of gun
	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* Mesh;

	// particle which is Muzzle flash
	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;

	// max range for line trace - line
	UPROPERTY(EditAnywhere)
	float MaxRange = 1000;

	// hit impact - Particle effect
	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditAnywhere)
	float Damage = 10.f;

	bool GunTrace(FHitResult &Hit, FVector &ShotDirection);
	AController* GetOwnerController() const;

	// sound effect of Muzzle flash
	UPROPERTY(EditAnywhere)
	class USoundBase* MuzzleFlashSound;

	// sound effect for hit impact
	UPROPERTY(EditAnywhere)
	USoundBase* ImpactSound;
};
