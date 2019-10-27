// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PT_ArenaPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PIXOTRONIC_API APT_ArenaPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APT_ArenaPlayerController();

	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* Pawn) override;

	UFUNCTION()
	void LeaveGame();

protected:
	virtual void BeginPlay() override;

private:
	static const FName ReturnBinding;
};
