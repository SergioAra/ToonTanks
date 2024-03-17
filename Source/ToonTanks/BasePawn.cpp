// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"

#include "Projectile.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABasePawn::ABasePawn():
	TurretTurnRate(10.f)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	SetRootComponent(CapsuleComponent);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComponent);
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
	
	
}

void ABasePawn::HandleDestruction()
{
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BaseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TurretMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if(DeathParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());
	}
	if(DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}
	if(DeathCameraShake)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShake);
	}
}

// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABasePawn::RotateTurret(FVector LookAtTarget, float DeltaTime)
{
	//Rotation is set in world rotation
	float ToTarget = (LookAtTarget - TurretMesh->GetComponentLocation()).Rotation().Yaw;
	float TurretYaw = TurretMesh->GetComponentRotation().Yaw;
	FRotator LookAtRotation;
	if((ToTarget<0) == (TurretYaw<0))
	{
		LookAtRotation = FRotator(0.f,FMath::FInterpTo(TurretYaw, ToTarget, DeltaTime, TurretTurnRate) , 0.f);
	}
	else
	{
		LookAtRotation = FRotator(0.f,ToTarget , 0.f);
	}
	 
	TurretMesh->SetWorldRotation(LookAtRotation);
}

void ABasePawn::Fire()
{
	if(!ProjectileSpawnPoint) return;
	if(!Projectile) return;
	AProjectile* FiredProjectile = GetWorld()->SpawnActor<AProjectile>(Projectile, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
	FiredProjectile->SetOwner(this);
}

// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	

}

