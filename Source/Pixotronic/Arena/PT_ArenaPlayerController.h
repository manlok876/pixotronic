// Copyright 2019 Denis Maximenko

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PT_Structs.h"
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

	UFUNCTION(Server, Reliable, WithValidation)
	void SetColor(const FLinearColor& NewColor);

  UFUNCTION(Server, WithValidation, Reliable, BlueprintCallable)
  void ChooseModel(const FPT_BikeModel& BikeModel);

	UFUNCTION(Server, WithValidation, Reliable, BlueprintCallable)
	void NotifyReadiness(bool IsReady);

	UFUNCTION()
	void LeaveGame();

protected:
	virtual void BeginPlay() override;

private:
	static const FName ReturnBinding;
};
