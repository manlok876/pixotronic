// Fill out your copyright notice in the Description page of Project Settings.


#include "PT_TrailingComponent.h"

#include "DrawDebugHelpers.h"

UPT_TrailingComponent::UPT_TrailingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TrailWidth = 20.0f;
}

void UPT_TrailingComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UPT_TrailingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	DrawDebugLine(GetWorld(),
		GetComponentLocation(),
		GetComponentLocation() + FVector(0, 0, 100),
		FColor(0, 255, 0),
		true, -1, 0, 10);
}

void UPT_TrailingComponent::MakeTurnpoint()
{
}

