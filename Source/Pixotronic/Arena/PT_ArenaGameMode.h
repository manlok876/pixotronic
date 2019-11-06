// Copyright 2019 Denis Maximenko

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PT_ArenaGameMode.generated.h"

class APlayerStart;
class AController;
class APT_BaseBike;

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
	virtual void StartMatch() override;
	virtual void HandleMatchHasStarted() override;
	
	virtual APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;

	virtual bool ReadyToEndMatch_Implementation() override;
	virtual void HandleMatchHasEnded();

	virtual void RestartMatch();

	UFUNCTION()
	void RespawnPlayersAndStart();

	UFUNCTION(BlueprintCallable)
	void SetMaxPlayers(int NewMaxPlayers);

protected: 
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBikeCrash(APawn* Bike);

	UFUNCTION()
	FLinearColor GetColorForPlayerStart(AActor* PlayerStart);

private:
	bool WaitingRoundStart;

	int MaxPlayers;
	
	static const float RoundStartDelay;
	static const float ArenaClearDelay;

	int WinningScore;

	TMap<AController*, APlayerStart*> PlayerStartSpots;
	TSet<APlayerStart*> TakenStartSpots;
	TArray<FLinearColor> PlayerColors;

	UFUNCTION()
	void ClearArena();
};
