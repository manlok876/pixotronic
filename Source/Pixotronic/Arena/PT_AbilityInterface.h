// Copyright 2019 Denis Maximenko

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PT_AbilityInterface.generated.h"

UINTERFACE(Blueprintable)
class UPT_AbilityInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PIXOTRONIC_API IPT_AbilityInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Ability")
	void UseAbility();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Ability")
	void ApplyAbility();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Ability", meta = (DisplayName="Can Use?"))
	bool CanUse();
};
