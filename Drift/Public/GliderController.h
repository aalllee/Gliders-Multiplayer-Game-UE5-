// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GliderController.generated.h"

/**
 * 
 */
UCLASS()
class DRIFT_API AGliderController : public APlayerController
{
	GENERATED_BODY()
	
	//virtual void Tick(float DeltaSeconds) override;
public:
	void SetHUDHealth(float Health, float MaxHealth);
	virtual void OnPossess(APawn* InPawn) override;

protected:
	virtual void BeginPlay() override;
private:
	class AGliderHUD* GliderHUD;
};
