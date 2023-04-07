// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "GliderCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class UStaticMeshComponent;
class UCameraComponent;
class USpringArmComponent;
class UArrowComponent;

UCLASS()
class DRIFT_API AGliderCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGliderCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Move(const FInputActionValue& Value);
	void Steer(const FInputActionValue& Value);
	void LookAround(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void Dash(const FInputActionValue& Value);
	void Shoot(const FInputActionValue& Value);
	void ShootProjectile(const FInputActionValue& Value);
	void ChargeProjectile(const FInputActionValue& Value);
	void UpdateGunRotation();


	///
	/// GLIDER INPUT MAPPINGS
	/// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		UInputMappingContext* GliderMappingContext;

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

	///
	/// /////////////////////////
	/// 
	UPROPERTY(EditAnywhere, Category = "CustomPhysics")
	float accelerationScale = 100000.0f;

	

private:
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* GliderStaticMesh;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UArrowComponent* BeamGunSourceLeft;

	UPROPERTY(VisibleAnywhere)
	UArrowComponent* BeamGunSourceRight;

	UPROPERTY(EditAnywhere)
	float projectileChargeProgress = 0.0f;

};
