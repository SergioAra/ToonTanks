// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"

#include "Tank.h"
#include "Kismet/GameplayStatics.h"

ATower::ATower():
	FireRange(500.f),
	FireRate(2.f)
{
}

void ATower::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

		if(IsTankInRange())
		{
			RotateTurret(Tank->GetActorLocation(), DeltaSeconds);
		}
}

void ATower::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy();
}

void ATower::BeginPlay()
{
	Super::BeginPlay();

	Tank = Cast<ATank>( UGameplayStatics::GetPlayerPawn(this, 0));
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

bool ATower::IsTankInRange() const
{
	if(!Tank) return false;
	if(!Tank->bAlive) return false;
	float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
	return Distance <= FireRange;
	
}

void ATower::CheckFireCondition()
{
	if(IsTankInRange())
	{
		Fire();
	}
}
