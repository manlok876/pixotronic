// Fill out your copyright notice in the Description page of Project Settings.


#include "PT_ArenaGameState.h"

#include "Net/UnrealNetwork.h"

void APT_ArenaGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APT_ArenaGameState, CameraRef);
}