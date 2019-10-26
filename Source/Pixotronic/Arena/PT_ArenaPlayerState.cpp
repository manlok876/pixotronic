// Fill out your copyright notice in the Description page of Project Settings.


#include "PT_ArenaPlayerState.h"

#include "UnrealNetwork.h"

void APT_ArenaPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APT_ArenaPlayerState, PlayerColor);
}

void APT_ArenaPlayerState::SetColor_Implementation(const FLinearColor& NewColor)
{
	PlayerColor = NewColor;
}

bool APT_ArenaPlayerState::SetColor_Validate(const FLinearColor& NewColor)
{
	return true;
}
