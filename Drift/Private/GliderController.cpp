// Fill out your copyright notice in the Description page of Project Settings.


#include "GliderController.h"
#include "../HUD/GliderHUD.h"
#include "../HUD/CharacterOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Car.h"
/*
void AGliderController::Tick(float DeltaSeconds) {
	if (HasAuthority())
	{
	//	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Purple, GetName());
	//	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Purple, GetControlRotation().ToString());
	}
	else
	{
	//	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Emerald, GetName());
	//	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Emerald, GetControlRotation().ToString());
	
	}
	
}
*/


void AGliderController::BeginPlay()
{

	Super::BeginPlay();

	GliderHUD = Cast<AGliderHUD>(GetHUD());
}


void AGliderController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ACar* Glider = Cast<ACar>(InPawn);

	if (Glider)
	{
		SetHUDHealth(Glider->GetHealth(), Glider->GetMaxHealth());
	}
}



void AGliderController::SetHUDHealth(float Health, float MaxHealth)
{
	GliderHUD = (GliderHUD == nullptr) ? Cast<AGliderHUD>(GetHUD()) : GliderHUD;


	if (GliderHUD
		&& GliderHUD->CharacterOverlay
		&& GliderHUD->CharacterOverlay->HealthBar
		&& GliderHUD->CharacterOverlay->HealthText)
	{
		const float HealthPercent = Health / MaxHealth;
		GliderHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);
		FString HealthText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
		GliderHUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
	}

}
