// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "GameFramework/GameState.h"
#include "PT_ArenaGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRoundStartedDelegate);

/**
 * 
 */
UCLASS()
class PIXOTRONIC_API APT_ArenaGameState : public AGameState
{
	GENERATED_BODY()

public:
	APT_ArenaGameState();

	FOnRoundStartedDelegate OnRoundStarted;

protected:
	int RoundNumber;
};
