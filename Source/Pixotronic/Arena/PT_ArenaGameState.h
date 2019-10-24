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
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintPure)
	int GetMaxPlayerScore();

	UPROPERTY(BlueprintAssignable)
	FOnRoundStartedDelegate OnRoundStarted;

	UPROPERTY(Replicated)
	int RoundNumber;

	UPROPERTY(Replicated)
	TMap<class APT_ArenaPlayerController*, FColor> PlayerColors;

	UFUNCTION(BlueprintPure)
	int GetNumAlivePlayers();
	UFUNCTION(BlueprintCallable)
	void RegisterPlayerSpawn(APlayerController* Player);
	UFUNCTION(BlueprintCallable)
	void RegisterPlayerDeath(APlayerController* Player);

protected:
	UPROPERTY(Replicated)
	TSet<APlayerState*> AlivePlayers;
};
