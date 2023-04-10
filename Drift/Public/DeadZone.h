// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeadZone.generated.h"

UCLASS()
class DRIFT_API ADeadZone : public AActor
{
	GENERATED_BODY()
	
public:	
	ADeadZone();
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void OnBoxOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult
		);

public:	
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* DeadZoneBox;
};
