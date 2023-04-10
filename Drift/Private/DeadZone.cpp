// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadZone.h"
#include "Components/BoxComponent.h"
#include "Car.h"

ADeadZone::ADeadZone()
{
	PrimaryActorTick.bCanEverTick = false;
	DeadZoneBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	DeadZoneBox->SetupAttachment(RootComponent);
	DeadZoneBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	DeadZoneBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADeadZone::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		DeadZoneBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		DeadZoneBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		DeadZoneBox->OnComponentBeginOverlap.AddDynamic(this, &ADeadZone::OnBoxOverlap);
	}
}

void ADeadZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADeadZone::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACar* Glider = Cast<ACar>(OtherActor);
	if (Glider)
	{
		Glider->isDead();
	}
}