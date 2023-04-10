// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UStaticMeshComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;
class USphereComponent;
UCLASS()
class DRIFT_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();
	virtual void Tick(float DeltaTime) override;
	void fire(const FVector& linearVelocity, const FVector& direction, const float charge);

	float getLifeSpan();
	void Destroyed() override;
	void moveSurroundingGliders();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	float MaxSpeed = 30000.0f;

	UPROPERTY(EditAnywhere)
	float MinSpeed = 10000.0f;

	UPROPERTY(EditAnywhere)
		float addGravity = -500.0f;


	//
	//RPCS
	//
	/*
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SpawnNiagaraSystem(UNiagaraSystem* niagaraSys,FVector Location, FRotator Rotation);
	bool Server_SpawnNiagaraSystem_Validate(UNiagaraSystem* niagaraSys, FVector Location, FRotator Rotation);
	void Server_SpawnNiagaraSystem_Implementation(UNiagaraSystem* niagaraSys, FVector Location, FRotator Rotation);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void MULTI_SpawnNiagaraSystem(UNiagaraSystem* niagaraSys, FVector Location, FRotator Rotation);
	bool MULTI_SpawnNiagaraSystem_Validate(UNiagaraSystem* niagaraSys, FVector Location, FRotator Rotation);
	void MULTI_SpawnNiagaraSystem_Implementation(UNiagaraSystem* niagaraSys, FVector Location, FRotator Rotation);
*/
private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* projectileMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* RangeOverlapCollider;

	UPROPERTY(EditAnywhere)
	USphereComponent* SphereCollider;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere)
	float lifespan = 3.0f;


	UPROPERTY(EditAnywhere, Category = "GunFireFX")
	UNiagaraSystem* ProjectileExplosion;

	UPROPERTY(EditAnywhere)
	float explosionRadius = 2500.f;

	UPROPERTY(EditAnywhere)
	float explosionStrength = 100000.f;
};
