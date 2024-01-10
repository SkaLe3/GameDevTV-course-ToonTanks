// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Projectile.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "Particles/ParticleSystem.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

void ABasePawn::HandleDestruction()
{
	if (DeathParticles)
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());
	if (DeathSound)
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	if (DeathCameraShakeClass)
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShakeClass);
	
}

void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	FVector toTraget = LookAtTarget - TurretMesh->GetComponentLocation();
	FRotator lookAtRotation = toTraget.Rotation();
	lookAtRotation.Pitch = 0.0f;
	lookAtRotation.Roll = 0.0f;

	TurretMesh->SetWorldRotation(
		FMath::RInterpTo(TurretMesh->GetComponentRotation(),
		lookAtRotation,
		UGameplayStatics::GetWorldDeltaSeconds(this),
		10.0f));
}

void ABasePawn::Fire()
{
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileClass,
		ProjectileSpawnPoint->GetComponentLocation(),
		ProjectileSpawnPoint->GetComponentRotation());
	Projectile->SetOwner(this);
}

