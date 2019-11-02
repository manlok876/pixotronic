// Copyright 2019 Denis Maximenko


#include "PT_ArenaGameState.h"

#include "GameFramework/PlayerState.h"
#include "TimerManager.h"
#include "UnrealNetwork.h"

APT_ArenaGameState::APT_ArenaGameState()
{
	bAlwaysRelevant = true;
	SetReplicates(true);

	RoundNumber = 0;
}

void APT_ArenaGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APT_ArenaGameState, RoundNumber);
	DOREPLIFETIME(APT_ArenaGameState, AlivePlayers);
}

int APT_ArenaGameState::GetMaxPlayerScore()
{
	float MaxScore = 0.0f;
	
	for (APlayerState* Player : PlayerArray)
	{
		if (Player->Score > MaxScore)
		{
			MaxScore = Player->Score;
		}
	}

	return (int) MaxScore;
}

void APT_ArenaGameState::ScheduleNextRound_Implementation()
{
	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(NextRoundTimer, this, &APT_ArenaGameState::StartRound, 3.0f);
	}
	else
	{
		GetWorldTimerManager().SetTimer(NextRoundTimer, 3.0f, false);
	}
}

void APT_ArenaGameState::StartRound_Implementation()
{
	if (NextRoundTimer.IsValid())
	{
		NextRoundTimer.Invalidate();
	}

	if (HasAuthority()) 
	{
		// Will get replicated to clients
		RoundNumber++;
	}
	OnRoundStarted.Broadcast();
}

void APT_ArenaGameState::EndRound_Implementation()
{
	OnRoundEnded.Broadcast();
}

float APT_ArenaGameState::GetSecondsUntilNextRound()
{
	return GetWorldTimerManager().GetTimerRemaining(NextRoundTimer);
}

int APT_ArenaGameState::GetNumAlivePlayers()
{
	return AlivePlayers.Num();
}

void APT_ArenaGameState::RegisterPlayerSpawn(APlayerController* Player)
{
	if (HasAuthority())
	{
		check(!AlivePlayers.Contains(Player->PlayerState));
		AlivePlayers.Add(Player->PlayerState);
	}
}

void APT_ArenaGameState::RegisterPlayerDeath(APlayerController* Player)
{
	if (HasAuthority())
	{
		check(AlivePlayers.Contains(Player->PlayerState));
		AlivePlayers.Remove(Player->PlayerState);
	}
}
