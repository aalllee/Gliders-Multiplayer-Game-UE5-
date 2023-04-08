// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"

#include "Car.generated.h"

class UInputMappingContext;
class UInputAction;
class UCapsuleComponent;
class UCameraComponent;
class USpringArmComponent;
class UArrowComponent;
class AGun;
class AProjectile;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class DRIFT_API ACar : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACar();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void Elim();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastElim();
	void isDead();
	FORCEINLINE bool IsElimmed() const { return bElimmed; }
	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

protected:
	
	virtual void BeginPlay() override;

	// INPUT RELATED FUNCTIONS
	void AccelerateStart(const FInputActionValue& Value);
	void AccelerateEnd(const FInputActionValue& Value);
	void SteerStart(const FInputActionValue& Value);
	void SteerEnd(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void ThrusterStart(const FInputActionValue& Value);
	void ThrusterEnd(const FInputActionValue& Value);
	void Dash(const FInputActionValue& Value);
	void Shoot(const FInputActionValue& Value);
	void ShootEnd(const FInputActionValue& Value);
	void ShootProjectile(const FInputActionValue& Value);
	void ChargeProjectile(const FInputActionValue& Value);
	void UpdateGunRotation();
	void LookAround(const FInputActionValue& Value);

	/// RPCS ////////////////////////////////////
	UFUNCTION(Server, Unreliable, WithValidation)
	void Server_AddImpulse(const FVector Impulse, UStaticMeshComponent* Mesh);

	UFUNCTION(Server, Unreliable)
	void Server_SetMeshRelativeRotation(float Yaw);

	UFUNCTION(Server, Unreliable)
	void Server_ProjectileSpawn(const FVector& direction, const FVector& spawnPos, const FVector& carVelocity, const float charge);

	UFUNCTION(Server, Unreliable)
	void Server_SpawnFireEffect(const FVector_NetQuantize& HitPoint, const FVector_NetQuantize& HitNormal);

	UFUNCTION(NetMulticast, Unreliable)
	void Multi_SpawnFireEffect(const FVector_NetQuantize& HitPoint, const FVector_NetQuantize& HitNormal);

	UFUNCTION(Server, Reliable)
	void Server_Accelerate(bool isMoving, float val);

	UFUNCTION(Server, Reliable)
	void Server_Steer(bool isSteering, float steerDir);

	UFUNCTION(Server, Reliable)
	void Server_ThrusterActivate(bool isActive);

	UFUNCTION(Server, Reliable)
	void SpawnGun();
	

protected:
	
	/// CUSTOM PHYSICS PROPERTIES ///////////////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomPhysics")
	float accScale = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomPhysics")
	float jumpScale = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomPhysics")
	float dashDist = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomPhysics")
	float torqueMag = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomPhysics")
	float gravity = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomPhysics")
	float ThrusterScale = 100000.f;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "CustomPhysics")
	bool isAirborn = false;

	UPROPERTY(EditAnywhere, Category = "Gun")
	TSubclassOf<AGun> BP_Gun;

	UPROPERTY(replicated)
	AGun* Gun_p;

	UPROPERTY(replicated)
	AGun* GunTest_p;

public:	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* Capsule;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* CarStaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* BeamGunSourceLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* BeamGunSourceRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* GliderBodyMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* GliderGunBeltMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float projectileChargeProgress = 0.0f;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	FRotator GunBeltRotation = FRotator(0, 0, 0);
	
private:
	bool projectileCooldown = false;
	bool IsAccelerating = false;
	float  accelerateInput = 0.0f;
	bool IsSteering = false;
	float  SteeringInput = 0.0f;
	bool ThrusterActive = false;

	UPROPERTY(EditAnywhere)
	UArrowComponent* ArrowComp;
	FTimerHandle FireTimer;
	UPROPERTY(EditAnywhere, Category = Combat)
	float FireDelay = .15f;
	bool canFire = true;
	bool isFireButtonDown = false;
	void FireTimerFinished();
	void StartFireTimer();
	UPROPERTY(EditAnywhere, Category = "GunFX")
	UNiagaraSystem* Beam;

	UPROPERTY(EditAnywhere, Category = "GunFX")
	UNiagaraSystem* SparkBurst;

	FTimerHandle ElimTimer;
	UPROPERTY(EditDefaultsOnly)
	float ElimDelay = 3.f;
	void ElimTimerFinished();

/// <summary>
/// HUD STUFF
/// </summary>
///
/// 
	UPROPERTY(EditAnywhere, Category="Player stats")
	float MaxHealth = 100.f;

	UPROPERTY(Replicated = OnRep_Health, VisibleAnywhere, Category = "Player stats")
	float Health = 100.f;
	UFUNCTION()
	void OnRep_Health();

	bool bElimmed = false;

	class AGliderController* GliderPlayerController;
protected:
	///INPUT ACTIONS //////////////////////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		UInputMappingContext* CarMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		UInputAction* SteerAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		UInputAction* DashAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		UInputAction* LookAroundAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		UInputAction* ShootProjectileAction;
};
