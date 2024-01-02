// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AGun;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	float GetHealthPercentage() const;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// binding input with shoot button, and call function `shoot`
	void Shoot();

private:
	
	// =========================== methods for keyboard axis Input mapping==================================
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUp(float AxisValue);
	void LookRight(float AxisValue);

	void Jump() override;
	// ======================================================================================================

	// ========================= variables and method for Gamepad Input Binding =============================
	
	// rotation rate for gamepay right Thumbstics for both x and y axis
	UPROPERTY(EditAnywhere)
	float RotationRate = 10;

	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);
	// ======================================================================================================

	// ==================== Spawn Gun =======================================================================
	// this TSubclassOf will help to assign Blueprint of gun class in editor to assign
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> GunClass;

	// this variable hold the blueprint type of gun 
	UPROPERTY()
	AGun* Gun;

	// ======================================================================================================

	
	// ======================== Health ======================================================================
	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere)
	float Health = 0;

	// ======================================================================================================
};
