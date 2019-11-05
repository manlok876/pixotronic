// Copyright 2019 Denis Maximenko

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PT_Structs.h"
#include "PT_ArenaPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PIXOTRONIC_API APT_ArenaPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	APT_ArenaPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated)
	FLinearColor PlayerColor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FPT_BikeModel ChosenBike;

	UPROPERTY(Replicated)
	bool ReadyToStart;
};
