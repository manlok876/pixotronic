// Fill out your copyright notice in the Description page of Project Settings.


#include "PT_TrailingComponent.h"

#include "UObject/ConstructorHelpers.h"
#include "DrawDebugHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"


const FName UPT_TrailingComponent::ColorParameterName("BaseColor");

UPT_TrailingComponent::UPT_TrailingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TrailWidth = 20.0f;
	Color = FColor(255, 255, 255);
	CurrentInstance = -1;

	// TODO: create material
	static ConstructorHelpers::FObjectFinder<UMaterial> TrailMaterial(TEXT("/Game/Assets/Materials/TrailMaterial.TrailMaterial"));
	if (TrailMaterial.Succeeded())
	{
		SetMaterial(0, TrailMaterial.Object);
	}
}

void UPT_TrailingComponent::BeginPlay()
{
	Super::BeginPlay();
	SetColor(Color);
}


void UPT_TrailingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// TODO: replace with InstancedMesh-based logic
	DrawDebugLine(GetWorld(),
		GetComponentLocation(),
		GetComponentLocation() + FVector(0, 0, 100),
		FColor(0, 255, 0),
		true, -1, 0, 10);
}

void UPT_TrailingComponent::SetColor(const FColor& NewColor)
{
	Color = NewColor;
	UMaterialInstanceDynamic* NewMaterial = UMaterialInstanceDynamic::Create(GetMaterial(0), this);
	if (NewMaterial != nullptr)
	{
		NewMaterial->SetVectorParameterValue(ColorParameterName, FLinearColor(NewColor));
		SetMaterial(0, NewMaterial);
	}
}

void UPT_TrailingComponent::MakeTurnpoint()
{
}

void UPT_TrailingComponent::TurnOn()
{
}

void UPT_TrailingComponent::TurnOff()
{
}

