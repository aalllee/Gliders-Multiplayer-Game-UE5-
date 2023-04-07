// Fill out your copyright notice in the Description page of Project Settings.


#include "GliderCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
// Sets default values
AGliderCharacter::AGliderCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GliderStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CARMESH"));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	
	SetRootComponent(GliderStaticMesh);
	SpringArm->SetupAttachment(GliderStaticMesh);
	Camera->SetupAttachment(SpringArm);

}

// Called when the game starts or when spawned
void AGliderCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			InputSubsystem->AddMappingContext(GliderMappingContext, 0);
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Black, TEXT("playerControllerSetup"));
		}
	}
	
}

void AGliderCharacter::Move(const FInputActionValue& Value)
{
	AddMovementInput(FVector(0, 0, 1), Value.Get<float>());
}

void AGliderCharacter::Steer(const FInputActionValue& Value)
{
}

void AGliderCharacter::LookAround(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Black, FString::SanitizeFloat(Value.Get<FVector2D>().X));
	AddControllerYawInput(Value.Get<FVector2D>().X * -1);
	AddControllerPitchInput(Value.Get<FVector2D>().Y);
}

void AGliderCharacter::Jump(const FInputActionValue& Value)
{
}

void AGliderCharacter::Dash(const FInputActionValue& Value)
{
}

// Called every frame
void AGliderCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGliderCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGliderCharacter::Move);
		EnhancedInputComponent->BindAction(LookAroundAction, ETriggerEvent::Triggered, this, &AGliderCharacter::LookAround);
		/*
		EnhancedInputComponent->BindAction(SteerAction, ETriggerEvent::Triggered, this, &ACar::Steer);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACar::Jump);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &ACar::Dash);
		EnhancedInputComponent->BindAction(LookAroundAction, ETriggerEvent::Triggered, this, &ACar::LookAround);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ACar::Shoot);
		EnhancedInputComponent->BindAction(ShootProjectileAction, ETriggerEvent::Canceled, this, &ACar::ShootProjectile);
		EnhancedInputComponent->BindAction(ShootProjectileAction, ETriggerEvent::Completed, this, &ACar::ShootProjectile);
		EnhancedInputComponent->BindAction(ShootProjectileAction, ETriggerEvent::Ongoing, this, &ACar::ChargeProjectile);
		*/
	}
}

