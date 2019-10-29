// Copyright 2019 Denis Maximenko

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PT_StartupGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PIXOTRONIC_API APT_StartupGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APT_StartupGameMode();

	// This is mostly for testing purposes
	UFUNCTION(BlueprintCallable)
	void SinglePlay();

	UFUNCTION(BlueprintCallable)
	void HostGame();

	UFUNCTION(BlueprintCallable)
	void JoinGame();

private:

	static const FName ArenaLevel;
};
