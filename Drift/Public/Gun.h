// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"
class UNiagaraSystem;
class UNiagaraComponent;
class ACar;
class AProjectile;
UCLASS()
class DRIFT_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	AGun();
	virtual void Tick(float DeltaTime) override;

	FHitResult Shoot(const FVector& traceStart, const FVector& traceDir, const FVector& leftGun, const FVector& rightGun);
	void ShootProjectile(const FVector& direction, const FVector& spawnPos, const FVector& carVelocity, const float charge);

	UFUNCTION(Server, Reliable)
	void Server_AddOnHitImpulse(const FVector_NetQuantize& Impulse, ACar* hitCar);

	UPROPERTY(EditAnywhere, Category = "GunSettings")
	float hitImpulse = 100000.0f;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "GunSettings")
	float gunRange = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<AProjectile> BP_Projectile;

};
