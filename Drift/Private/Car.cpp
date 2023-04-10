// Fill out your copyright notice in the Description page of Project Settings.


#include "Car.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Gun.h"
#include "Projectile.h"
#include "Components/ArrowComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"
#include "GliderController.h"
#include "../GameMode/GliderGameMode.h"

ACar::ACar()
{
	PrimaryActorTick.bCanEverTick = true;
	CarStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CARMESH"));
	GliderBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GLIDERBODY"));
	GliderGunBeltMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunBelt"));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint"));

	BeamGunSourceLeft = CreateDefaultSubobject<UArrowComponent>(TEXT("BeamGunSourceLeft"));
	BeamGunSourceRight = CreateDefaultSubobject<UArrowComponent>(TEXT("BeamGunSourceRight"));

	SetRootComponent(CarStaticMesh);
		GliderBodyMesh->SetupAttachment(CarStaticMesh);
			GliderGunBeltMesh->SetupAttachment(GliderBodyMesh);
				BeamGunSourceLeft->SetupAttachment(GliderGunBeltMesh);
				BeamGunSourceRight->SetupAttachment(GliderGunBeltMesh);
				ArrowComp->SetupAttachment(GliderGunBeltMesh);
		SpringArm->SetupAttachment(CarStaticMesh);
			Camera->SetupAttachment(SpringArm);
}
void ACar::SpawnGun_Implementation()
{

	if (UWorld* World = GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		Gun_p = World->SpawnActor<AGun>(BP_Gun, FVector(0, 0, 0), FRotator(0, 0, 0), SpawnParams);
	}

}
void ACar::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(CarMappingContext, 0);
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Black, TEXT("playerControllerSetup"));
		}
	}

	GliderPlayerController = Cast<AGliderController>(Controller);
	if (GliderPlayerController)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1000.f, FColor::Yellow, TEXT("FROM car begin play set HUD HEALTH CALLED"));
	//	GliderPlayerController->SetHUDHealth(Health, MaxHealth);
	}


	SpawnGun();

}
void ACar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		if(!Gun_p)
			GEngine->AddOnScreenDebugMessage(-1, 8, FColor::Red, TEXT("gun null tick"));

		if (IsAccelerating)
		{
			CarStaticMesh->AddForce(CarStaticMesh->GetForwardVector() * accScale * accelerateInput);
		}
		if (IsSteering)
		{
			CarStaticMesh->AddTorqueInRadians(CarStaticMesh->GetUpVector() * torqueMag * SteeringInput);
		}
		if (ThrusterActive)
		{
			CarStaticMesh->AddForce(FVector(0.f, 0.f, 1.f * ThrusterScale));
		}
	}

	UpdateGunRotation();


	if (projectileCooldown) {
		projectileChargeProgress -= 0.5f * DeltaTime;

		if (projectileChargeProgress < 0.0f)
		{
			projectileChargeProgress = 0.f;
			projectileCooldown = false;
		}
		if (GliderPlayerController)
		{
			GliderPlayerController->SetHUDCharge(projectileChargeProgress);
		}
		
	}

}
void ACar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Accelerate Forward/Backward
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &ACar::AccelerateStart);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ACar::AccelerateEnd);
		//Steer Left/Right
		EnhancedInputComponent->BindAction(SteerAction, ETriggerEvent::Started, this, &ACar::SteerStart);
		EnhancedInputComponent->BindAction(SteerAction, ETriggerEvent::Completed, this, &ACar::SteerEnd);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACar::Jump);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACar::ThrusterStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACar::ThrusterEnd);


		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &ACar::Dash);

		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &ACar::Shoot);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &ACar::ShootEnd);


		EnhancedInputComponent->BindAction(ShootProjectileAction, ETriggerEvent::Canceled, this, &ACar::ShootProjectile);
		EnhancedInputComponent->BindAction(ShootProjectileAction, ETriggerEvent::Completed, this, &ACar::ShootProjectile);
		EnhancedInputComponent->BindAction(ShootProjectileAction, ETriggerEvent::Ongoing, this, &ACar::ChargeProjectile);


		//local
		EnhancedInputComponent->BindAction(LookAroundAction, ETriggerEvent::Triggered, this, &ACar::LookAround);
	}
}

void ACar::Elim()
{
	MulticastElim();
	GetWorldTimerManager().SetTimer(
		ElimTimer,
		this,
		&ACar::ElimTimerFinished,
		ElimDelay
	);
}
void ACar::ElimTimerFinished()
{
	//only on server since called from gamemode object

	AGliderGameMode* GliderGameMode = nullptr;

	if (UWorld* World = GetWorld())
	{
		GliderGameMode = World->GetAuthGameMode<AGliderGameMode>();
	}

	if (GliderGameMode)
	{
		GliderGameMode->RequestRespawn(this, Controller);
	//	GliderGameMode->RestartPlayer(Controller);
	}

}

void ACar::MulticastElim_Implementation()
{
	bElimmed = true;
	if(GEngine)
	GEngine->AddOnScreenDebugMessage(-1, 8, FColor::Red, GetName() + FString(": Eliminated"));

	if (GliderPlayerController)
	{
		DisableInput(GliderPlayerController);
	}


}

void ACar::isDead()
{
	AGliderGameMode* GliderGameMode = nullptr;
	if(UWorld* World = GetWorld())
		GliderGameMode = GetWorld()->GetAuthGameMode<AGliderGameMode>();

	if (GliderGameMode)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 8, FColor::Yellow, FString("ACar::isDead"));
		GliderPlayerController = (GliderPlayerController == nullptr) ? Cast<AGliderController>(Controller) : GliderPlayerController;
		GliderGameMode->PlayerEliminated(this, GliderPlayerController);

	}
	else {
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 8, FColor::Yellow, FString("GLIDERGAMEMODE NULL"));
	}
}


void ACar::AccelerateStart(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 8, FColor::Red, TEXT("MoveStart"));
	IsAccelerating = true;
	accelerateInput = Value.Get<float>();
	if (!HasAuthority())
		Server_Accelerate(true, accelerateInput);
}
void ACar::AccelerateEnd(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 8, FColor::Red, TEXT("MoveEnd"));
	accelerateInput = 0.f;
	IsAccelerating = false;
	if (!HasAuthority())
		Server_Accelerate(false, 0.f);
}

void ACar::SteerStart(const FInputActionValue& Value)
{
	IsSteering = true;
	SteeringInput = Value.Get<float>();

	if(!HasAuthority())
		Server_Steer(true, SteeringInput);
}
void ACar::SteerEnd(const FInputActionValue& Value)
{
	IsSteering = false;
	SteeringInput = Value.Get<float>();
	if (!HasAuthority())
		Server_Steer(false, SteeringInput);
}

void ACar::Jump(const FInputActionValue& Value)
{
	if (HasAuthority())
	{
		CarStaticMesh->AddImpulse(FVector(0.f, 0.f, 1.f * jumpScale));
	}
	else
	{
		Server_AddImpulse(FVector(0.f, 0.f, 1.f * jumpScale), CarStaticMesh);
	}
}
void ACar::ThrusterStart(const FInputActionValue& Value)
{
	ThrusterActive = true;
	
	if (!HasAuthority())
	{
		Server_ThrusterActivate(ThrusterActive);
	}

}
void ACar::ThrusterEnd(const FInputActionValue& Value)
{
	ThrusterActive = false;

	if (!HasAuthority())
	{
		Server_ThrusterActivate(ThrusterActive);
	}
}
void ACar::Dash(const FInputActionValue& Value)
{

}

void ACar::Shoot(const FInputActionValue& Value)
{

	
	isFireButtonDown = true;
	if (Gun_p && canFire) {

		canFire = false;
		StartFireTimer();
		FHitResult HitResult = Gun_p->Shoot(Camera->GetComponentLocation(), GetControlRotation().Vector(),
													BeamGunSourceLeft->GetComponentTransform().GetLocation(),
													BeamGunSourceRight->GetComponentTransform().GetLocation());

		
		Server_SpawnFireEffect(HitResult.ImpactPoint, HitResult.Normal);
	}	
	

	if (!Gun_p)
	{
		GEngine->AddOnScreenDebugMessage(-1, 8, FColor::Red, TEXT("SHOOT Gun NUll"));
	}

}
void ACar::ShootEnd(const FInputActionValue& Value)
{
	isFireButtonDown = false;
}
void ACar::FireTimerFinished()
{
	canFire = true;
	if (isFireButtonDown)
	{
		GEngine->AddOnScreenDebugMessage(-1, 8, FColor::Red, TEXT("FireFinished callback"));
		Shoot(FInputActionValue(1.0f));
	}
}
void ACar::StartFireTimer()
{
	GetWorldTimerManager().SetTimer(FireTimer, this, &ACar::FireTimerFinished, FireDelay);
}

void ACar::OnRep_Health()
{
}
void ACar::Server_SpawnFireEffect_Implementation(const FVector_NetQuantize& HitPoint, const FVector_NetQuantize& HitNormal)
{
	Multi_SpawnFireEffect(HitPoint, HitNormal);
}
void ACar::Multi_SpawnFireEffect_Implementation(const FVector_NetQuantize& HitPoint, const FVector_NetQuantize& HitNormal)
{
	if (Beam && SparkBurst) {
		UNiagaraComponent* LeftGunEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Beam, BeamGunSourceLeft->GetComponentTransform().GetLocation(), FRotator(0.f));
		LeftGunEffect->SetNiagaraVariableVec3(FString("beamEnd"), HitPoint);

		UNiagaraComponent* RightGunEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Beam, BeamGunSourceRight->GetComponentTransform().GetLocation(), FRotator(0.f));
		RightGunEffect->SetNiagaraVariableVec3(FString("beamEnd"), HitPoint);

		UNiagaraComponent* RightGunSparks = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SparkBurst, HitPoint, FRotator(0.f));
		RightGunEffect->SetNiagaraVariableVec3(FString("BurstDirection"), HitNormal);

		
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 8, FColor::Red, FString("Beam FX null"));
	}
}
void ACar::Server_ProjectileSpawn_Implementation(const FVector& direction, const FVector& spawnPos, const FVector& carVelocity, const float charge)
{
	if(Gun_p)
		Gun_p->ShootProjectile(direction, spawnPos, carVelocity, charge);
}

void ACar::ShootProjectile(const FInputActionValue& Value)
{
	if (projectileCooldown)
		return;

	if (HasAuthority())
	{
		FVector spawnPoint = ArrowComp->GetComponentTransform().GetTranslation();
		if (Gun_p) {
			Gun_p->ShootProjectile(GetControlRotation().Vector(), spawnPoint, CarStaticMesh->GetComponentVelocity(), projectileChargeProgress);
			projectileCooldown = true;
		}
	}
	else {
		
		FVector spawnPoint = ArrowComp->GetComponentTransform().GetTranslation();
		if (Gun_p) {
			Server_ProjectileSpawn(GetControlRotation().Vector(), spawnPoint, CarStaticMesh->GetComponentVelocity(), projectileChargeProgress);
			projectileCooldown = true;
		}

	}

	

	
}
void ACar::ChargeProjectile(const FInputActionValue& Value)
{
	if (projectileCooldown)
		return;

	if (UWorld* world_p = GetWorld()) {
		projectileChargeProgress += 1.f * UGameplayStatics::GetWorldDeltaSeconds(world_p);
	}

	if(GliderPlayerController)
		GliderPlayerController->SetHUDCharge(projectileChargeProgress);
}
void ACar::UpdateGunRotation()
{
	if (UWorld* World = GetWorld())
	{
		APawn* playerPawn = UGameplayStatics::GetPlayerPawn(World, 0);
		if (playerPawn)
		{
			if (dynamic_cast<ACar*>(playerPawn) == this)
			{
				double remaped = FMath::GetMappedRangeValueUnclamped(FVector2D(0.f, 360.f), FVector2D(-180.f, 180.f), GetControlRotation().Yaw);
				GunBeltRotation = FRotator(0.f, remaped - CarStaticMesh->GetRelativeRotation().Yaw + 180.f, 0.f);
				Server_SetMeshRelativeRotation(remaped - CarStaticMesh->GetRelativeRotation().Yaw + 180.f);

				if (IsLocallyControlled())
				{
					GliderGunBeltMesh->SetRelativeRotation(FRotator(0.f, remaped - CarStaticMesh->GetRelativeRotation().Yaw + 180.f, 0.f));
				}

			}
		}
	}
}
void ACar::LookAround(const FInputActionValue& Value)
{
	AddControllerYawInput(Value.Get<FVector2D>().X * -1);
	AddControllerPitchInput(Value.Get<FVector2D>().Y);
}
void ACar::Server_SetMeshRelativeRotation_Implementation(float Yaw)
{
	GliderGunBeltMesh->SetRelativeRotation(FRotator(0.f, Yaw, 0.f));
}
void ACar::Server_ThrusterActivate_Implementation(bool isActive)
{
	ThrusterActive = isActive;
}
void ACar::Server_Steer_Implementation(bool isSteering, float steerDir)
{
	IsSteering = isSteering;
	SteeringInput = steerDir;
}
void ACar::Server_Accelerate_Implementation(bool isMoving, float val)
{
	IsAccelerating = isMoving;
	accelerateInput = val;
}
bool ACar::Server_AddImpulse_Validate(const FVector Impulse, UStaticMeshComponent* Mesh)
{
	return true;
}
void ACar::Server_AddImpulse_Implementation(const FVector Impulse, UStaticMeshComponent* Mesh)
{
	Mesh->AddImpulse(Impulse);
}
void ACar::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ACar, Gun_p, COND_OwnerOnly);
	
}