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
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 8, FColor::Yellow, FString("AGliderGameMode::PlayerEliminated"));
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
		GEngine->AddOnScreenDebugMessage(-1, 8, FColor::Green, FString("RESTART PLAYER"));
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
		RestartPlayerAtPlayerStart(ElimmedController, PlayerStarts[Selection]);
	
	}
}
/*
void AGliderGameMode::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
}





AActor* AGliderGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	
	//return Super::ChoosePlayerStart_Implementation(Player);
	
	
	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();

	GEngine->AddOnScreenDebugMessage(-1, 8, FColor::Green, FString::FromInt(NumberOfPlayers));
	

	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
	
	return PlayerStarts[NumberOfPlayers-1];
	
}
*/