// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Car.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Projectile.h"

AGun::AGun()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}
void AGun::BeginPlay()
{
	Super::BeginPlay();
}
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
FHitResult AGun::Shoot(const FVector& traceStart, const FVector& traceDir, const FVector& leftGun, const FVector& rightGun)
{
	FHitResult Hit;

	FVector TraceStart = traceStart;
	FVector TraceEnd = traceStart + traceDir * gunRange;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, QueryParams);

	if (Hit.bBlockingHit && IsValid(Hit.GetActor()))
	{
		if (ACar* car_p = dynamic_cast<ACar*>(Hit.GetActor())) {
			Server_AddOnHitImpulse(traceDir * hitImpulse, car_p);
		}
	}
	else {
		//no hit
		Hit.ImpactPoint = FVector_NetQuantize(TraceEnd);
		Hit.ImpactNormal = FVector_NetQuantize(FVector(0));
	}
	return Hit;
}
void AGun::ShootProjectile(const FVector& direction, const FVector& spawnPos, const FVector& carVelocity, const float charge)
{
	FVector Location = direction;
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	if (GetWorld()) {
		AProjectile* projectile_p = GetWorld()->SpawnActor<AProjectile>(BP_Projectile, spawnPos, FRotator(0, 180.f, 0));
		projectile_p->fire(carVelocity, direction, charge);
	}
}
void AGun::Server_AddOnHitImpulse_Implementation(const FVector_NetQuantize& Impulse, ACar* hitCar)
{
	if(hitCar)
		hitCar->CarStaticMesh->AddImpulse(Impulse);
}



