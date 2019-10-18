// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
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
	FColor Color;

	UFUNCTION(BlueprintCallable)
	void SetColor(const FColor& NewColor);

	UFUNCTION(BlueprintCallable)
	void MakeTurnpoint();

	// Enables trailing
	UFUNCTION()
	void TurnOn();

	// Disables trailing
	UFUNCTION()
	void TurnOff();

protected:
	virtual void BeginPlay() override;

	static const FName ColorParameterName;

private:
	UPROPERTY()
	TArray<FVector> TurnPoints;

	UPROPERTY()
	int CurrentInstance;
};
