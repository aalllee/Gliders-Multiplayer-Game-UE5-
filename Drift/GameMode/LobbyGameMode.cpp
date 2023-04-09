// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	int32 NumberOfPlayers =  GameState.Get()->PlayerArray.Num();

	TArray < AActor*, FDefaultAllocator> actorList;
	NewPlayer->GetSeamlessTravelActorList(false, actorList);
	actorList.Add(NewPlayer->GetPawn());
	
	for (auto actor : actorList)
	{
		actorList[0]->GetName();
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 1000.f, FColor::Turquoise, FString("Car name") + NewPlayer->GetName());

	if (NumberOfPlayers == 2)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			bUseSeamlessTravel = true;
			World->ServerTravel(FString("/Game/Maps/testLevel1?listen"));
		}
	}
}
