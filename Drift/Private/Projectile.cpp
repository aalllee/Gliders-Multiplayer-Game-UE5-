// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Car.h"
#include "GameFramework/ProjectileMovementComponent.h"
// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	projectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	
	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));

	SetRootComponent(projectileMesh);
	
	SphereCollider->SetupAttachment(projectileMesh);
	CollisionBox->SetupAttachment(projectileMesh);
	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;


}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(lifespan);

}

bool AProjectile::Server_SpawnNiagaraSystem_Validate(UNiagaraSystem* niagaraSys, FVector Location, FRotator Rotation)
{
	return true;
}

void AProjectile::Server_SpawnNiagaraSystem_Implementation(UNiagaraSystem* niagaraSys, FVector Location, FRotator Rotation)
{
	GEngine->AddOnScreenDebugMessage(-1, 8, FColor::Red, TEXT("Server"));
	MULTI_SpawnNiagaraSystem(niagaraSys, Location, Rotation);
	//UNiagaraComponent* NiagaraProjectileExplosion = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), niagaraSys, Location, Rotation);
}

bool AProjectile::MULTI_SpawnNiagaraSystem_Validate(UNiagaraSystem* niagaraSys, FVector Location, FRotator Rotation)
{
	return true;
}

void AProjectile::MULTI_SpawnNiagaraSystem_Implementation(UNiagaraSystem* niagaraSys, FVector Location, FRotator Rotation)
{
	GEngine->AddOnScreenDebugMessage(-1, 8, FColor::Red, TEXT("Multicast"));
	GEngine->AddOnScreenDebugMessage(-1, 8, FColor::Red,FString("spawn at")+ Location.ToString());

	UNiagaraComponent* NiagaraProjectileExplosion = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), niagaraSys, Location, Rotation);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	projectileMesh->AddForce(FVector(0,0,addGravity));
}

void AProjectile::applyPhysics(const FVector impulse)
{
	projectileMesh->AddImpulse(impulse);
}

void AProjectile::fire(const FVector& linearVelocity, const FVector& direction, const float charge)
{
	float speed = FMath::GetMappedRangeValueClamped(FVector2D(0.f, 1.f), FVector2D(MinSpeed, MaxSpeed), charge);

	projectileMesh->SetPhysicsLinearVelocity(linearVelocity);
	projectileMesh->AddImpulse(direction * speed);
}

void AProjectile::setVelocity(const FVector& startPos, const FVector& direction)
{


}

void AProjectile::getIsActive()
{
	
}

float AProjectile::getLifeSpan()
{
	return this->lifespan;
}

void AProjectile::Destroyed()
{
	if (ProjectileExplosion)
	{
		UNiagaraComponent* NiagaraProjectileExplosion = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ProjectileExplosion, GetActorLocation(), FRotator(0.f));
	}
	
	if (HasAuthority())
	{
		moveSurroundingGliders();
	}
	
	Super::Destroyed();
}	

void AProjectile::moveSurroundingGliders()
{

	TSet<AActor*> GlidersInRange;
	TSubclassOf<ACar*> targetClass;
	SphereCollider->GetOverlappingActors(GlidersInRange);

	for (auto glider : GlidersInRange)
	{
		GEngine->AddOnScreenDebugMessage(-1, 8, FColor::Red,FString("Projectile in range of: ") + *(glider->GetName()));
		FVector impulse = glider->GetActorLocation() - GetActorLocation();
		impulse.Normalize();
		dynamic_cast<ACar*>(glider)->CarStaticMesh->AddImpulse(explosionStrength*impulse);
	}
}

