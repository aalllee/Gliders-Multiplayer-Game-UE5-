// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GliderHUD.generated.h"

class UTexture2D;
/**
 * 
 */
UCLASS()
class DRIFT_API AGliderHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

	UPROPERTY(EditAnywhere, Category = "user widgets")
	TSubclassOf<class UUserWidget> CharacterOverlayClass;
	UPROPERTY(EditAnywhere, Category = "user widgets")
	TSubclassOf<UUserWidget> BP_Crosshair;

	class UCharacterOverlay* CharacterOverlay;
	class UUserWidget* Crosshair;

protected:
	virtual void BeginPlay() override;
	void AddCharacterOverlay();
private:
	void DrawCrosshair(FVector2D ViewportCenter);
	
	UPROPERTY(EditAnywhere, Category = "TEXTURES")
	UTexture2D* CrosshairTexture;

	
};
