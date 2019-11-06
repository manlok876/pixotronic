// Copyright 2019 Denis Maximenko


#include "PT_ArenaGameMode.h"

#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

#include "PT_AbilityBike.h"
#include "PT_ArenaPlayerController.h"
#include "PT_ArenaPlayerState.h"
#include "PT_ArenaGameState.h"

const float APT_ArenaGameMode::RoundStartDelay = 5.0f;

APT_ArenaGameMode::APT_ArenaGameMode() 
{
	DefaultPawnClass = APT_AbilityBike::StaticClass();
	PlayerControllerClass = APT_ArenaPlayerController::StaticClass();
	PlayerStateClass = APT_ArenaPlayerState::StaticClass();
	GameStateClass = APT_ArenaGameState::StaticClass();
	
	bDelayedStart = true;
	SetMaxPlayers(2);
	WinningScore = 5;
	WaitingRoundStart = false;

	PlayerColors.Add(FLinearColor(1.0f, 1.0f, 1.0f));
	PlayerColors.Add(FLinearColor(1.0f,    0,    0));
	PlayerColors.Add(FLinearColor(   0, 1.0f,    0));
	PlayerColors.Add(FLinearColor(1.0f, 1.0f,    0));
	PlayerColors.Add(FLinearColor(   0,    0, 1.0f));
}

void APT_ArenaGameMode::BeginPlay()
{
	APT_ArenaGameState* ArenaGameState = GetGameState<APT_ArenaGameState>();
	check(IsValid(ArenaGameState));
	ArenaGameState->OnRoundStarted.AddDynamic(this, &APT_ArenaGameMode::RespawnPlayersAndStart);
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
	if (WaitingRoundStart)
	{
		return false;
	}

	if (GetNumPlayers() != MaxPlayers)
	{
		return false;
	}

	APT_ArenaGameState* ArenaGameState = GetGameState<APT_ArenaGameState>();
	check(IsValid(ArenaGameState));

	for (auto Player : ArenaGameState->PlayerArray)
	{
		APT_ArenaPlayerState* ArenaPlayer = CastChecked<APT_ArenaPlayerState>(Player);

		if (!ArenaPlayer->ReadyToStart)
		{
			return false;
		}
	}

    return true;
}

void APT_ArenaGameMode::StartMatch()
{
	UE_LOG(LogTemp, Display, TEXT("MyStartMatch"));
	APT_ArenaGameState* ArenaGameState = GetGameState<APT_ArenaGameState>();
	check(IsValid(ArenaGameState));
	ArenaGameState->ScheduleNextRound(RoundStartDelay);
	WaitingRoundStart = true;
	ArenaGameState->StartMatch();
}

void APT_ArenaGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
}

APawn* APT_ArenaGameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	APawn* PlayerPawn = Super::SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);

	APT_AbilityBike* PlayerBike = Cast<APT_AbilityBike>(PlayerPawn);
	if (IsValid(PlayerBike))
	{
		APT_ArenaPlayerState* ArenaPlayer = Cast<APT_ArenaPlayerState>(NewPlayer->PlayerState);
		if (IsValid(ArenaPlayer))
		{
			PlayerBike->ApplyBikeModel(ArenaPlayer->ChosenBike);
		}
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
		ArenaGameState->EndMatch();
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
	APT_ArenaGameState* ArenaGameState = GetGameState<APT_ArenaGameState>();
	check(IsValid(ArenaGameState));
	ArenaGameState->ScheduleNextRound(RoundStartDelay);
}

void APT_ArenaGameMode::RespawnPlayersAndStart()
{
	Super::StartMatch();
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
