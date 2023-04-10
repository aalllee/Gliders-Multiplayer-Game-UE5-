// Fill out your copyright notice in the Description page of Project Settings.


#include "GliderHUD.h"
#include "GameFramework/PlayerController.h"
#include "CharacterOverlay.h"
#include "Blueprint/UserWidget.h"
#include "../Public/GliderController.h"
void AGliderHUD::DrawHUD()
{
	Super::DrawHUD();
}

void AGliderHUD::BeginPlay()
{
	Super::BeginPlay();
	AddCharacterOverlay();
}

void AGliderHUD::AddCharacterOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController)
	{

		if (BP_Crosshair)
		{
			Crosshair = CreateWidget<UUserWidget>(PlayerController, BP_Crosshair);
			Crosshair->AddToViewport();
		}
		if (CharacterOverlayClass)
		{
			CharacterOverlay = CreateWidget<UCharacterOverlay>(PlayerController, CharacterOverlayClass);
			CharacterOverlay->AddToViewport();
		}
	}
}


