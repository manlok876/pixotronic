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
	ChangeBikeMesh(Model.Mesh);

	if (IsValid(AbilityComponent))
	{
		AbilityComponent->DestroyComponent();
	}

	if (Model.AbilityComponentClass == nullptr)
	{
		return;
	}
	AbilityComponent = NewObject<UActorComponent>(this, Model.AbilityComponentClass.Get(), *Model.Name);
	if (IsValid(AbilityComponent))
	{
		AbilityComponent->RegisterComponent();
	}
}

bool APT_AbilityBike::ApplyBikeModel_Validate(const FPT_BikeModel& Model)
{
	return true;
}

void APT_AbilityBike::ChangeBikeMesh_Implementation(UStaticMesh* NewMesh)
{
	if (IsValid(MeshComponent) && IsValid(NewMesh))
	{
		MeshComponent->SetStaticMesh(NewMesh);
	}
}
