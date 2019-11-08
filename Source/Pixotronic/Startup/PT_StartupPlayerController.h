// Copyright 2019 Denis Maximenko

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PT_StartupPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBackButtonPressedDelegate);

/**
 * 
 */
UCLASS()
class PIXOTRONIC_API APT_StartupPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetupInputComponent() override;

	UPROPERTY(BlueprintAssignable)
	FOnBackButtonPressedDelegate OnBackButtonPressed;

private:
	UFUNCTION()
	void HandleBackPressed();
};
