// Copyright 2019 Denis Maximenko

#pragma once

#include "CoreMinimal.h"
#include "Arena/PT_BaseBike.h"
#include "PT_AbilityBike.generated.h"

/**
 * 
 */
UCLASS()
class PIXOTRONIC_API APT_AbilityBike : public APT_BaseBike
{
	GENERATED_BODY()

public:
	APT_AbilityBike();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Replicated)
	UActorComponent* AbilityComponent;
};
