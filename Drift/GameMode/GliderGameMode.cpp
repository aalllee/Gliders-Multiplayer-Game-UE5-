// Fill out your copyright notice in the Description page of Project Settings.


#include "GliderGameMode.h"
#include "../Public/Car.h"
#include "../Public/GliderController.h"
void AGliderGameMode::PlayerEliminated(ACar* ElimmedCharacter, AGliderController* ElimController)
{
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Elim();
	}
}
