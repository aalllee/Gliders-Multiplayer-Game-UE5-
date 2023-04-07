// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "BlasterCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class URotatingMovementComponent;
class AWeapon;

UCLASS()
class DRIFT_API ABlasterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABlasterCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;
	//FORCEINLINE void SetOverlappingWeapon(AWeapon* Weapon) { OverlappingWeapon = Weapon; }
	void SetOverlappingWeapon(AWeapon* Weapon);
protected:
	virtual void BeginPlay() override;
	void MoveFE(const FInputActionValue& Value);
	void LookAround(const FInputActionValue& Value);
	void Steer(const FInputActionValue& Value);
	void SteerCompleted(const FInputActionValue& Value);
	void EquipButtonPressed(const FInputActionValue& Value);

	//INPUT ACTIONS
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAroundAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* CharMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SteerAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* EquipAction;

	/////////////////////////////////////////////////////////////////////


	UFUNCTION(Server, Reliable)
	void Server_Steer(float val);



	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float turnRate = 0.f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float turnAcceleration = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float turnResist = 1.f;

	UPROPERTY(VisibleAnywhere)
	class UCombatComponent* Combat;

private:
	URotatingMovementComponent* SteeringRotationComponent;

	bool isSteering = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess="true"))
	class UWidgetComponent* OverheadWidget;

	UPROPERTY(ReplicatedUsing=OnRep_OverlappingWeapon)
	class AWeapon* OverlappingWeapon;

	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	UFUNCTION(SERVER, Reliable)
	void ServerEquipButtonPressed();
};
