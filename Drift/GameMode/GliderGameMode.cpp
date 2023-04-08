// Fill out your copyright notice in the Description page of Project Settings.


#include "GliderGameMode.h"
#include "../Public/Car.h"
#include "../Public/GliderController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
void AGliderGameMode::PlayerEliminated(ACar* ElimmedCharacter, AGliderController* ElimController)
{
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Elim();
	}
}

void AGliderGameMode::RequestRespawn(ACar* ElimmedCharacter, AController* ElimmedController)
{
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Reset();
		ElimmedCharacter->Destroy();
	}

	if (ElimmedController)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
		RestartPlayerAtPlayerStart(ElimmedController, PlayerStarts[Selection]);
	
	}
}

void AGliderGameMode::PostLogin(APlayerController* NewPlayer)
{
}

void AGliderGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	if (NewPlayer)
	{
		GEngine->AddOnScreenDebugMessage(-1, 8, FColor::Green, NewPlayer->GetName());
	}
	
}
