// Fill out your copyright notice in the Description page of Project Settings.


#include "PT_TrailingComponent.h"

#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"


const FName UPT_TrailingComponent::ColorParameterName("Color");

UPT_TrailingComponent::UPT_TrailingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TrailWidth = 20.0f;
	TrailHeight = 100.0f;
	TrailStartOffset = FVector(0.0f);

	Color = FLinearColor::FromSRGBColor(FColor(255, 255, 255, 255));

	bIsTrailingEnabled = false;
	CurrentInstance = -1;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TrailMesh(TEXT("/Game/Assets/Meshes/Cube.Cube"));
	if (TrailMesh.Succeeded())
	{
		SetStaticMesh(TrailMesh.Object);
	}

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
	TurnOn();
	// Maybe use native delegate?
	//GetOwner()->GetRootComponent()->TransformUpdated.AddUObject(this, &UPT_TrailingComponent::OnTransformUpdated);
}

void UPT_TrailingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateTrail();
}

void UPT_TrailingComponent::OnTransformUpdated(USceneComponent* Component, EUpdateTransformFlags UpdateFlags, ETeleportType TeleportType)
{
	static FRotator PreviousRotation = FRotator::ZeroRotator;
	UpdateTrail();
	if (PreviousRotation != Component->GetComponentRotation())
	{
		MakeTurnpoint();
	}
}

FTransform UPT_TrailingComponent::CalcNewSegmentTransform()
{
	FVector LastTurnPoint = TurnPoints[TurnPoints.Num() - 1];
	FVector NewLocation = (LastTurnPoint + GetTrailStartLocation()) / 2.0f;
	FRotator NewRotation = UKismetMathLibrary::MakeRotFromX(NewLocation - LastTurnPoint);
	FVector NewScale = 
		FVector(
			FVector::Distance(GetTrailStartLocation(), LastTurnPoint) + TrailWidth, TrailWidth, TrailHeight) / 100.0f;
	return FTransform(NewRotation, NewLocation, NewScale);
}

void UPT_TrailingComponent::UpdateTrail()
{
	if (bIsTrailingEnabled) {
		UpdateInstanceTransform(
			CurrentInstance, 
			CalcNewSegmentTransform(),
			true, true);
	}
}

void UPT_TrailingComponent::SetColor(const FLinearColor& NewColor)
{
	Color = NewColor;
	UMaterialInstanceDynamic* NewMaterial = UMaterialInstanceDynamic::Create(GetMaterial(0), this);
	if (NewMaterial != nullptr)
	{
		NewMaterial->SetVectorParameterValue(ColorParameterName, NewColor);
		SetMaterial(0, NewMaterial);
	}
}

void UPT_TrailingComponent::MakeTurnpoint_Implementation()
{
	if (bIsTrailingEnabled) 
	{
		EndCurrentSegment();
		StartNewSegment();
	}
}

void UPT_TrailingComponent::TurnOn_Implementation()
{
	if (bIsTrailingEnabled)
	{
		return;
	}
	bIsTrailingEnabled = true;
	MakeTurnpoint();
}

void UPT_TrailingComponent::TurnOff_Implementation()
{
	if (!bIsTrailingEnabled)
	{
		return;
	}
	EndCurrentSegment();
	bIsTrailingEnabled = false;
}

FVector UPT_TrailingComponent::GetTrailStartLocation()
{
	return GetOwner()->GetActorLocation() + TrailStartOffset;
}

void UPT_TrailingComponent::EndCurrentSegment()
{
	if (bIsTrailingEnabled && CurrentInstance >= 0)
	{
		check(CurrentInstance < GetInstanceCount());
		UpdateTrail();
		AddTurnPoint();
		CurrentInstance = -1;
	}
}

void UPT_TrailingComponent::StartNewSegment()
{
	int NewInstance =
		AddInstance(
			FTransform(
				FRotator::ZeroRotator,
				GetOwner()->GetActorLocation(),
				FVector(0, 0, 0)));
	if (NewInstance >= 0)
	{
		check(NewInstance > CurrentInstance);
		if (TurnPoints.Num() == 0)
		{
			AddTurnPoint();
		}
		CurrentInstance = NewInstance;
	}
}

void UPT_TrailingComponent::AddTurnPoint()
{
	TurnPoints.Add(GetOwner()->GetActorLocation());
}
