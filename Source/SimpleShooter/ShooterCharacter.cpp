// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	// spawn gun
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);

	// hide bone by name, we are actually here hiding the default weapon attached to character
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);

	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);
}

bool AShooterCharacter::IsDead() const
{
    return Health <= 0;
}

float AShooterCharacter::GetHealthPercentage() const
{
    return Health / MaxHealth;
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ===================== Binding Input ============================================================
	// bind axis like Moveforward and look up for character
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AShooterCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AShooterCharacter::LookRight);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Jump);
	
	// gamepad axis
	//PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	//PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AShooterCharacter::LookRightRate);

	// shoot binding
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);

	// =================================================================================================
}

float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
	float damageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	damageToApply = FMath::Min(Health, damageToApply);
	Health -= damageToApply;
	
	if(IsDead())
	{
		// fetch the game mode component before Detach call...
		ASimpleShooterGameModeBase* gameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		// call the in Game Mode
		if(gameMode != nullptr)
		{
			gameMode->PawnKilled(this);
		}
		
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	UE_LOG(LogTemp, Display, TEXT("Health = %f"), Health);

    return damageToApply;
}

void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void AShooterCharacter::LookRight(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

void AShooterCharacter::Jump()
{
	Super::Jump();
}

void AShooterCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::Shoot()
{
	if(Gun == nullptr)
		return;

	Gun->PullTrigger();
}
