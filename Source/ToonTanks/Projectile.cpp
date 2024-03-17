// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AProjectile::AProjectile():
	InitialSpeed(1300.f),
	MaxSpeed(1300.f),
	Damage(50.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	SetRootComponent(ProjectileMesh);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovementComponent->MaxSpeed = MaxSpeed;
	ProjectileMovementComponent->InitialSpeed = InitialSpeed;

	SmokeTrailComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Trail"));
	SmokeTrailComponent->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	if(ProjectileMesh)
	{
		ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	}
	if(LaunchSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
	}
	
	
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* OwnerActor = GetOwner();
	if(OwnerActor)
	{
		AController* OwnerController = OwnerActor->GetInstigatorController();

		if(OwnerController && OtherActor && OtherActor != this && OtherActor != OwnerActor)
		{
			UGameplayStatics::ApplyDamage(OtherActor, Damage, OwnerController, this, UDamageType::StaticClass());
			if(HitParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation(), GetActorRotation());
			}
			if(HitSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
			}
			if(HitCameraShake)
			{
				GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShake);
			}
		}
	}
	if(SmokeTrailComponent)
	{
		SmokeTrailComponent->Deactivate();
		SetActorEnableCollision(false);
		ProjectileMesh->SetVisibility(false);
		FTimerHandle TrailDelay;
		GetWorld()->GetTimerManager().SetTimer(TrailDelay, [&](){Destroy();}, 0.8f, false);
	}
	else
	{
		Destroy();
	}
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

