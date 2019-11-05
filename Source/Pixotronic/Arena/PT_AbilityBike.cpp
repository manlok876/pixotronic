// Copyright 2019 Denis Maximenko


#include "PT_AbilityBike.h"

#include "UnrealNetwork.h"

APT_AbilityBike::APT_AbilityBike() : AbilityComponent(nullptr)
{
}

void APT_AbilityBike::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APT_AbilityBike, AbilityComponent);
}
