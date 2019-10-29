// Copyright 2019 Denis Maximenko


#include "PT_ArenaGameMode.h"

#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

#include "PT_BaseBike.h"
#include "PT_ArenaPlayerController.h"
#include "PT_ArenaPlayerState.h"
#include "PT_ArenaGameState.h"

APT_ArenaGameMode::APT_ArenaGameMode() 
{
	DefaultPawnClass = APT_BaseBike::StaticClass();
	PlayerControllerClass = APT_ArenaPlayerController::StaticClass();
	PlayerStateClass = APT_ArenaPlayerState::StaticClass();
	GameStateClass = APT_ArenaGameState::StaticClass();
	
	bDelayedStart = true;
	SetMaxPlayers(2);
	RoundStartDelay = 5.0f;
	WinningScore = 5;

	PlayerColors.Add(FLinearColor(1.0f, 1.0f, 1.0f));
	PlayerColors.Add(FLinearColor(1.0f,    0,    0));
	PlayerColors.Add(FLinearColor(   0, 1.0f,    0));
	PlayerColors.Add(FLinearColor(   0,    0, 1.0f));
	PlayerColors.Add(FLinearColor(   0, 1.0f, 1.0f));
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

			// Assign player a color along with start spot
			APT_ArenaPlayerState* ArenaPlayerState =
				Cast<APT_ArenaPlayerState>(Player->PlayerState);
			if (IsValid(ArenaPlayerState))
			{
				ArenaPlayerState->PlayerColor = GetColorForPlayerStart(*StartItr);
			}

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

	APT_ArenaGameState* ArenaGameState = GetGameState<APT_ArenaGameState>();
	check(IsValid(ArenaGameState));
	ArenaGameState->StartRound();
}

APawn* APT_ArenaGameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	APawn* PlayerPawn = Super::SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);

	APT_BaseBike* PlayerBike = Cast<APT_BaseBike>(PlayerPawn);
	if (IsValid(PlayerBike))
	{
		APT_ArenaGameState* ArenaGameState = GetGameState<APT_ArenaGameState>();
		if (IsValid(ArenaGameState))
		{
			ArenaGameState->AlivePlayers.Add(NewPlayer->PlayerState);
			PlayerBike->OnDeath.AddDynamic(this, &APT_ArenaGameMode::OnBikeCrash);
		}
	}
	else
	{
		UE_LOG(LogPlayerManagement, Error, TEXT("Failed to spawn bike for player"));
	}
	
	return PlayerPawn;
}

bool APT_ArenaGameMode::ReadyToEndMatch_Implementation()
{
	APT_ArenaGameState* ArenaGameState = GetGameState<APT_ArenaGameState>();
	check(IsValid(ArenaGameState));

	return ArenaGameState->GetNumAlivePlayers() <= 1;
}

void APT_ArenaGameMode::HandleMatchHasEnded()
{
	APT_ArenaGameState* ArenaGameState = GetGameState<APT_ArenaGameState>();
	check(IsValid(ArenaGameState));

	ArenaGameState->EndRound();

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
			PlayerPawn->Destroy();
			PCItr->SetPawn(nullptr);
		}
	}
	StartMatch();
}

void APT_ArenaGameMode::SetMaxPlayers(int NewMaxPlayers)
{
	MaxPlayers = NewMaxPlayers;
}

void APT_ArenaGameMode::OnBikeCrash(APawn* Bike)
{
	APT_ArenaGameState* ArenaGameState = GetGameState<APT_ArenaGameState>();
	check(IsValid(ArenaGameState));

	APlayerState* CrashedPlayer = Bike->GetPlayerState();
	check(IsValid(CrashedPlayer));

	ArenaGameState->AlivePlayers.Remove(CrashedPlayer);
}

FLinearColor APT_ArenaGameMode::GetColorForPlayerStart(AActor* PlayerStart)
{
	FLinearColor ResultColor(1.0f, 1.0f, 1.0f);
	
	if (IsValid(PlayerStart))
	{
		FString PlayerStartName = FString(PlayerStart->GetName());
		int StartPointNumber = 0;
		if (PlayerStartName.Contains("1"))
		{
			StartPointNumber = 1;
		}
		else if (PlayerStartName.Contains("2"))
		{
			StartPointNumber = 2;
		}
		else if (PlayerStartName.Contains("3"))
		{
			StartPointNumber = 3;
		}
		else if (PlayerStartName.Contains("4"))
		{
			StartPointNumber = 4;
		}

		if (PlayerColors.IsValidIndex(StartPointNumber))
		{
			ResultColor = PlayerColors[StartPointNumber];
		}
	}

	return ResultColor;
}
