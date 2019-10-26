// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PT_ArenaPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PIXOTRONIC_API APT_ArenaPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated)
	FColor PlayerColor;

	UFUNCTION(Server, Reliable, WithValidation)
	void SetColor(const FColor& NewColor);
};
