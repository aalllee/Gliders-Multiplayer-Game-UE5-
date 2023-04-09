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
	GEngine->AddOnScreenDebugMessage(-1, 1000.f, FColor::Yellow, TEXT("Add character overlay called"));
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
			//Cast<AGliderController>(PlayerController)->SetHUDHealth(100.f, 100.f);
		}
	}
}

void AGliderHUD::DrawCrosshair(FVector2D ViewportCenter)
{
	const float TextureW = CrosshairTexture->GetSizeX();
	const float TextureH = CrosshairTexture->GetSizeY();
	const FVector2D TextureDrawPoint(
		ViewportCenter.X - (TextureW / 2.f),
		ViewportCenter.Y - (TextureH / 2.f));

	DrawTexture(
		CrosshairTexture,
		ViewportCenter.X - (TextureW / 2.f),
		ViewportCenter.Y,
		TextureW,
		TextureH,
		0.f,
		0.f,
		1.f,
		1.f,
		FLinearColor::White,
		BLEND_Translucent,
		1.f
	);

	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && BP_Crosshair)
	{
		
	
			//PlayerController->CreateWidget
			
	}


}

