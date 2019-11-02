// Copyright 2019 Denis Maximenko

#pragma once

#include "CoreMinimal.h"
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

	UFUNCTION(NetMulticast, Reliable)
	void ScheduleNextRound();

	UFUNCTION(NetMulticast, Reliable)
	void StartRound();
	UPROPERTY(BlueprintAssignable)
	FOnRoundStartedDelegate OnRoundStarted;

	UFUNCTION(NetMulticast, Reliable)
	void EndRound();
	UPROPERTY(BlueprintAssignable)
	FOnRoundStartedDelegate OnRoundEnded;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated)
	int RoundNumber;
	UFUNCTION(BlueprintPure)
	float GetSecondsUntilNextRound();

	UFUNCTION(BlueprintPure)
	int GetNumAlivePlayers();
	UFUNCTION(BlueprintCallable)
	void RegisterPlayerSpawn(APlayerController* Player);
	UFUNCTION(BlueprintCallable)
	void RegisterPlayerDeath(APlayerController* Player);

	UPROPERTY(Replicated)
	TSet<APlayerState*> AlivePlayers;

private:
	FTimerHandle NextRoundTimer;
};
