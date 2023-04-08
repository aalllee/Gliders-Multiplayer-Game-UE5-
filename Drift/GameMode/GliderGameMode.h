// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GliderGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DRIFT_API AGliderGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	virtual void PlayerEliminated(class ACar* ElimmedCharacter, class AGliderController* ElimController);
};
