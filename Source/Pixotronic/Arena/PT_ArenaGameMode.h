// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PT_ArenaGameMode.generated.h"

class APlayerStart;
class AController;

/**
 * 
 */
UCLASS()
class PIXOTRONIC_API APT_ArenaGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	APT_ArenaGameMode();

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	
	virtual bool ReadyToStartMatch_Implementation() override;
	virtual void HandleMatchHasStarted() override;
	
	virtual APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;

	virtual bool ReadyToEndMatch_Implementation() override;
	virtual void HandleMatchHasEnded();

	virtual void RestartMatch();

	UFUNCTION(BlueprintCallable)
	void SetMaxPlayers(int NewMaxPlayers);

protected: 
	UFUNCTION()
	void OnBikeCrash(APawn* Bike);

private:
	int MaxPlayers;
	
	float RoundStartDelay;
	FTimerHandle RoundStartTimer;

	int WinningScore;

	TMap<AController*, APlayerStart*> PlayerStartSpots;
	TSet<APlayerStart*> TakenStartSpots;
};
