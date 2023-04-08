// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadZone.h"
#include "Components/BoxComponent.h"
#include "Car.h"
// Sets default values
ADeadZone::ADeadZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DeadZoneBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));


	
	DeadZoneBox->SetupAttachment(RootComponent);
	DeadZoneBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	DeadZoneBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ADeadZone::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		GEngine->AddOnScreenDebugMessage(-1, 8, FColor::Yellow, FString("begin"));
		DeadZoneBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		DeadZoneBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		DeadZoneBox->OnComponentBeginOverlap.AddDynamic(this, &ADeadZone::OnBoxOverlap);
		//DeadZoneBox->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnSphereEndOverlap);
	}

}

// Called every frame
void ADeadZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADeadZone::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	GEngine->AddOnScreenDebugMessage(-1, 8, FColor::Yellow, FString("overlaped dead zone"));

	ACar* Glider = Cast<ACar>(OtherActor);
	if (Glider)
	{
	
	
	}
	
}