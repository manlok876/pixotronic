// Copyright 2019 Denis Maximenko


#include "PT_AbilityBike.h"

#include "UnrealNetwork.h"
#include "Engine/StaticMesh.h"
#include "Components/StaticMeshComponent.h"

APT_AbilityBike::APT_AbilityBike() : AbilityComponent(nullptr)
{
}

void APT_AbilityBike::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APT_AbilityBike, AbilityComponent);
}

void APT_AbilityBike::ApplyBikeModel_Implementation(const FPT_BikeModel& Model)
{
	if (IsValid(MeshComponent) && IsValid(Model.Mesh))
	{
		MeshComponent->SetStaticMesh(Model.Mesh);
	}

	if (IsValid(AbilityComponent))
	{
		AbilityComponent->DestroyComponent();
	}

	AbilityComponent = NewObject<UActorComponent>(this, Model.AbilityComponentClass.Get(), Model.Name);
	if (IsValid(AbilityComponent))
	{
		AbilityComponent->SetIsReplicated(true);
		AbilityComponent->RegisterComponent();
	}
}

bool APT_AbilityBike::ApplyBikeModel_Validate(const FPT_BikeModel& Model)
{
	return true;
}
