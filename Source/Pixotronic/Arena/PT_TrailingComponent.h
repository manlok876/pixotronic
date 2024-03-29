// Copyright 2019 Denis Maximenko

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "PT_TrailingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PIXOTRONIC_API UPT_TrailingComponent : public UInstancedStaticMeshComponent
{
	GENERATED_BODY()

public:	
	UPT_TrailingComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TrailWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TrailHeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor Color;

	UFUNCTION(BlueprintCallable)
	void SetColor(const FLinearColor& NewColor);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void MakeTurnpoint();

	// Enables trailing
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void TurnOn();

	// Disables trailing
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void TurnOff();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector TrailStartOffset;

	UFUNCTION(BlueprintPure)
	FVector GetTrailStartLocation();

protected:
	virtual void BeginPlay() override;

	static const FName ColorParameterName;

	UFUNCTION()
	void UpdateTrail();

	void OnTransformUpdated(USceneComponent* Component, EUpdateTransformFlags UpdateFlags, ETeleportType TeleportType);

private:
	UPROPERTY()
	bool bIsTrailingEnabled;

	UPROPERTY()
	TArray<FVector> TurnPoints;

	UPROPERTY()
	int CurrentInstance;

	UFUNCTION()
	FTransform CalcNewSegmentTransform();

	UFUNCTION()
	void EndCurrentSegment();
	UFUNCTION()
	void StartNewSegment(bool ConnectedToPrevious = true);

	UFUNCTION()
	void AddTurnPoint();
};
