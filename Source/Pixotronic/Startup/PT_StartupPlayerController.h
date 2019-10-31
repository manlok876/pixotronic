// Copyright 2019 Denis Maximenko

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PT_StartupPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PIXOTRONIC_API APT_StartupPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APT_StartupPlayerController();

	UFUNCTION(Server, WithValidation, Reliable)
	void SetReadyToStart(bool IsReady);

	UFUNCTION()
	bool GetReadyToStart();

private:
	UPROPERTY()
	bool ReadyToStart;
};
