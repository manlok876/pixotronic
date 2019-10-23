// Fill out your copyright notice in the Description page of Project Settings.


#include "PT_ArenaGameMode.h"

#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
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
}

AActor* APT_ArenaGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	for (TActorIterator<APlayerStart> StartItr(GetWorld()); StartItr; ++StartItr)
	{
		break;
	}
	return Super::ChoosePlayerStart_Implementation(Player);
}

bool APT_ArenaGameMode::ReadyToStartMatch_Implementation()
{
	return (Super::ReadyToStartMatch_Implementation() && GetNumPlayers() == MaxPlayers);
}

void APT_ArenaGameMode::HandleMatchHasStarted()
{
	//for (TActorIterator<APlayerController> PCItr(GetWorld()); PCItr; ++PCItr)
	//{
	//	PCItr->SetPause(true);
	//}
	Super::HandleMatchHasStarted();
}

bool APT_ArenaGameMode::ReadyToEndMatch_Implementation()
{
	//return GetGameState<APT_ArenaGameState>();
	return false;
}

void APT_ArenaGameMode::HandleMatchHasEnded()
{
	APT_ArenaGameState* GameState = GetGameState<APT_ArenaGameState>();
}

void APT_ArenaGameMode::RestartMatch()
{
}

void APT_ArenaGameMode::SetMaxPlayers(int NewMaxPlayers)
{
	MaxPlayers = NewMaxPlayers;
}
