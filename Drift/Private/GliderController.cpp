// Fill out your copyright notice in the Description page of Project Settings.


#include "GliderController.h"
#include "../HUD/GliderHUD.h"
#include "../HUD/CharacterOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Car.h"

void AGliderController::BeginPlay()
{
	Super::BeginPlay();
	GliderHUD = Cast<AGliderHUD>(GetHUD());
}
void AGliderController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}
void AGliderController::SetHUDCharge(float Charge)
{
	GliderHUD = (GliderHUD == nullptr) ? Cast<AGliderHUD>(GetHUD()) : GliderHUD;
	if (GliderHUD
		&& GliderHUD->CharacterOverlay
		&& GliderHUD->CharacterOverlay->ProjectileChargeBar)
	{
		GliderHUD->CharacterOverlay->ProjectileChargeBar->SetPercent(Charge);
	}
}
