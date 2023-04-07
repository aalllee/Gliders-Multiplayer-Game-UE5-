// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "../Weapon/Weapon.h"
#include "../BlasterComponents/CombatComponent.h"
// Sets default values
ABlasterCharacter::ABlasterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SteeringRotationComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotationComp"));
	SteeringRotationComponent->RotationRate = FRotator(0.f, 0.f, 0.f);
	
	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);

	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));
	Combat->SetIsReplicated(true);
}

void ABlasterCharacter::MoveFE(const FInputActionValue& Value)
{
	const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
	const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
	
	AddMovementInput(GetCapsuleComponent()->GetForwardVector(), Value.Get<float>());
	//GetCharacterMovement()->AddForce(GetCapsuleComponent()->GetForwardVector()*100000.f);

	
}

void ABlasterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ABlasterCharacter, OverlappingWeapon,COND_OwnerOnly);

}

void ABlasterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (Combat)
	{
		Combat->Character = this;
	}

}


void ABlasterCharacter::BeginPlay()
{
	Super::BeginPlay();
	

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{

		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			InputSubsystem->AddMappingContext(CharMappingContext, 0);
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Black, TEXT("playerControllerSetup"));
		}

	}
	SteeringRotationComponent->RotationRate.Yaw = 0.0f;
	
}



void ABlasterCharacter::LookAround(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Black, FString::SanitizeFloat(Value.Get<FVector2D>().X));
	AddControllerYawInput(Value.Get<FVector2D>().X * -1);
	AddControllerPitchInput(Value.Get<FVector2D>().Y);
}

void ABlasterCharacter::Steer(const FInputActionValue& Value)
{
	
	//GetCapsuleComponent()->AddLocalRotation(FRotator(0.f, turnRate*Value.Get<float>(), 0.f));
	isSteering = true;
	if(UWorld* World = GetWorld())
		turnRate += turnAcceleration*Value.Get<float>()*UGameplayStatics::GetWorldDeltaSeconds(World);
	
	

	if (!HasAuthority())
	{
		//Server_Steer_Implementation(turnRate);
		GEngine->AddOnScreenDebugMessage(-1, 8, FColor::Red, GetName());
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 8, FColor::Green, GetName());
		//SteeringRotationComponent->RotationRate.Yaw = turnRate;
	}

	
}

void ABlasterCharacter::SteerCompleted(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 8, FColor::Red, FString::SanitizeFloat(Value.Get<float>()));
	GEngine->AddOnScreenDebugMessage(-1, 8, FColor::Red, FString("COMPLETED"));
	isSteering = false;
}

void ABlasterCharacter::EquipButtonPressed(const FInputActionValue& Value)
{
	if (Combat)
	{
		if (HasAuthority())
		{
			Combat->EquipWeapon(OverlappingWeapon);
		}
		else
		{
			ServerEquipButtonPressed();
		}
	}

}

void ABlasterCharacter::SetOverlappingWeapon(AWeapon* Weapon) {
	if (IsLocallyControlled())
	{
		if (OverlappingWeapon)
		{
			OverlappingWeapon->ShowPickupWidget(false);
		}
	}
	OverlappingWeapon = Weapon;

	if (IsLocallyControlled())
	{
		if (OverlappingWeapon)
		{
			OverlappingWeapon->ShowPickupWidget(true);
		}
	}
	
}

void ABlasterCharacter::OnRep_OverlappingWeapon(AWeapon* LastWeapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(true);
	}
	if (LastWeapon)
	{
		LastWeapon->ShowPickupWidget(false);
	}
}

void ABlasterCharacter::ServerEquipButtonPressed_Implementation()
{
	if (Combat)
	{
		Combat->EquipWeapon(OverlappingWeapon);

	}

}

void ABlasterCharacter::Server_Steer_Implementation(float val)
{
	//SteeringRotationComponent->RotationRate.Yaw = val;
	GetCapsuleComponent()->AddLocalRotation(FRotator(0, 1, 0));
}

// Called every frame
void ABlasterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 8, FColor::Red, FString::SanitizeFloat(turnRate));
	


	if (HasAuthority())
	{
		
		GEngine->AddOnScreenDebugMessage(-1, 8, FColor::Red, GetBaseAimRotation().ToString());

		//GetMovementComponent()->
	}

	if (!isSteering)
	{
		if (turnRate > 0.1f)
		{
			turnRate -= turnResist * DeltaTime;
		}
		else if (turnRate < -0.1f)
		{
			turnRate += turnResist * DeltaTime;
		}
	}
	SteeringRotationComponent->RotationRate.Yaw = turnRate;
}

// Called to bind functionality to input
void ABlasterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
//	PlayerInputComponent->BindAxis("MoveF", this, &ABlasterCharacter::MoveF);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABlasterCharacter::MoveFE);
		EnhancedInputComponent->BindAction(LookAroundAction, ETriggerEvent::Triggered, this, &ABlasterCharacter::LookAround);
		EnhancedInputComponent->BindAction(SteerAction, ETriggerEvent::Triggered, this, &ABlasterCharacter::Steer);
		EnhancedInputComponent->BindAction(SteerAction, ETriggerEvent::Completed, this, &ABlasterCharacter::SteerCompleted);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Started, this, &ABlasterCharacter::EquipButtonPressed);
	}


}



