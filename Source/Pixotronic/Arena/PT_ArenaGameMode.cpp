// Fill out your copyright notice in the Description page of Project Settings.


#include "PT_ArenaGameMode.h"

#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

#include "PT_BaseBike.h"
#include "PT_ArenaPlayerController.h"
#include "PT_ArenaGameState.h"

APT_ArenaGameMode::APT_ArenaGameMode() 
{
	DefaultPawnClass = APT_BaseBike::StaticClass();
	PlayerControllerClass = APT_ArenaPlayerController::StaticClass();
	GameStateClass = APT_ArenaGameState::StaticClass();
	
	bDelayedStart = true;
	SetMaxPlayers(2);
	RoundStartDelay = 5.0f;
	WinningScore = 5;
}

AActor* APT_ArenaGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	if (PlayerStartSpots.Find(Player) != nullptr)
	{
		return PlayerStartSpots[Player];
	}

	for (TActorIterator<APlayerStart> StartItr(GetWorld()); StartItr; ++StartItr)
	{
		if (!TakenStartSpots.Contains(*StartItr))
		{
			PlayerStartSpots.Add(Player, *StartItr);
			TakenStartSpots.Add(*StartItr);
			return *StartItr;
		}
	}

	// If no start points found
	return Super::ChoosePlayerStart_Implementation(Player);
}

bool APT_ArenaGameMode::ReadyToStartMatch_Implementation()
{
	return GetNumPlayers() == MaxPlayers;
}

void APT_ArenaGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
	
	// Maybe pause all controllers and resume after delay
}

APawn* APT_ArenaGameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	APawn* PlayerPawn = Super::SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);
	
	APT_BaseBike* PlayerBike = Cast<APT_BaseBike>(PlayerPawn);
	if (IsValid(PlayerBike))
	{
		// Set color based on starting spot
	}
	
	return PlayerPawn;
}

bool APT_ArenaGameMode::ReadyToEndMatch_Implementation()
{
	return GetGameState<APT_ArenaGameState>()->GetNumAlivePlayers() <= 1;
}

void APT_ArenaGameMode::HandleMatchHasEnded()
{
	APT_ArenaGameState* ArenaGameState = GetGameState<APT_ArenaGameState>();
	
	for (APlayerState* PlayerState : ArenaGameState->AlivePlayers)
	{
		PlayerState->Score += 1.0;
	}

	if (ArenaGameState->GetMaxPlayerScore() < WinningScore)
	{
		RestartMatch();
	}
	else 
	{
		// Do nothing, maybe display a message
	}
}

void APT_ArenaGameMode::RestartMatch()
{
	SetMatchState(MatchState::WaitingToStart);
	for (TActorIterator<APlayerController> PCItr(GetWorld()); PCItr; ++PCItr)
	{
		APawn* PlayerPawn = PCItr->GetPawn();
		if (IsValid(PlayerPawn))
		{
			// Destroy pawn so that it is re-created at start of new round
			// to avoid messing with
			//PlayerPawn->Destroy();
			//PCItr->SetPawn(nullptr);
		}
	}
	StartMatch();
}

void APT_ArenaGameMode::SetMaxPlayers(int NewMaxPlayers)
{
	MaxPlayers = NewMaxPlayers;
}
