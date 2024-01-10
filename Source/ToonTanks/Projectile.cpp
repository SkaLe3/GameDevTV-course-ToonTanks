// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Tower.h"
#include "Tank.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile"));
	RootComponent = ProjectileMesh;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	TrailParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail Particles"));
	TrailParticles->SetupAttachment(RootComponent);

	ProjectileMovementComponent->MaxSpeed = 1300.f;
	ProjectileMovementComponent->InitialSpeed = 1300.f;

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	if (LaunchSound)
		UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
	
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* MyOwner = GetOwner();
	if (!MyOwner)
	{
		Destroy();
		return;
	}
	if (OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwner->GetInstigatorController(), this, UDamageType::StaticClass());
		if (HitSound)
			UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
		if (HitParticles)
			UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation(), GetActorRotation());
		if (OtherActor->IsA<ATower>())
		{
			if (HitMarker)
				UGameplayStatics::PlaySoundAtLocation(this, HitMarker, GetActorLocation());
		}
		if (GetOwner()->IsA<ATank>() || OtherActor->IsA<ATank>())
			if (HitCameraShakeClass)
			{
				GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
			}

	}
	Destroy();
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

