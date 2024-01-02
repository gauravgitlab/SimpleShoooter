// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

void AGun::PullTrigger()
{
	// spawn Muzzle flash Paticles from socket attached to Rifle Mesh...
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));

	// play sound
	UGameplayStatics::SpawnSoundAttached(MuzzleFlashSound, Mesh, TEXT("MuzzleFlashSocket"));

	FHitResult hitResult;
	FVector shotDirection;
	if(GunTrace(hitResult, shotDirection))
	{
		//DrawDebugPoint(GetWorld(), hit.Location, 20, FColor::Blue, true);
		//FVector shotDirection = -rotation.Vector();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, hitResult.Location, shotDirection.Rotation());

		// play hit sound
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, hitResult.Location);
		
		// fire damage event, to an actor who is going to recieve the damage
		AActor* hitActor = hitResult.GetActor();
		if(hitActor != nullptr)
		{
			AController* ownerController = GetOwnerController();
			if(ownerController == nullptr)
				return;

			FPointDamageEvent DamageEvent(Damage, hitResult, shotDirection, nullptr);
			hitActor->TakeDamage(Damage, DamageEvent, ownerController, this);
		}
	}
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGun::GunTrace(FHitResult &Hit, FVector &ShotDirection)
{
	AController* ownerController = GetOwnerController();
	if(ownerController == nullptr)
		return false;	

	FVector location;
	FRotator rotation;
	ownerController->GetPlayerViewPoint(location, rotation);

	// Draw debug camera, on particular location or ration of shooting, and Debug Point, both are at same location of camera
	// dont be confused, just use either of one...
	//DrawDebugCamera(GetWorld(), location, rotation, 90, 2, FColor::Red, true);
	//DrawDebugPoint(GetWorld(), location, 20, FColor::Red, true);

	// line trace
	FVector end = location + rotation.Vector() * MaxRange;

	// ignored actors from being hit to itself
	FCollisionQueryParams params;
	params.AddIgnoredActor(this); // gun
	params.AddIgnoredActor(GetOwner()); // pawn actor its own capsule collider
	
	// hit result of line trace
	bool success = GetWorld()->LineTraceSingleByChannel(Hit, location, end, ECollisionChannel::ECC_GameTraceChannel1, params);
	ShotDirection = -rotation.Vector();

    return success;
}

AController *AGun::GetOwnerController() const
{
    // get owner ( which is ShooterCharacter in this case)
	// and gets controller from it.... to use the `GetPlayerViewPoint` method
	// which provide camera location and rotation... 
	APawn* ownerPawn = Cast<APawn>(GetOwner());
	if(ownerPawn == nullptr) 
		return nullptr;

	AController* ownerController = ownerPawn->GetController();
	return ownerController;
}
