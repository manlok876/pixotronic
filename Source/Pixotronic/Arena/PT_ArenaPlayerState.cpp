// Copyright 2019 Denis Maximenko


#include "PT_ArenaPlayerState.h"

#include "UnrealNetwork.h"

APT_ArenaPlayerState::APT_ArenaPlayerState()
{
	bAlwaysRelevant = true;
	SetReplicates(true);
	ReadyToStart = false;
}

void APT_ArenaPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APT_ArenaPlayerState, PlayerColor);
	DOREPLIFETIME(APT_ArenaPlayerState, ReadyToStart);
	DOREPLIFETIME(APT_ArenaPlayerState, ChosenBike);
}
