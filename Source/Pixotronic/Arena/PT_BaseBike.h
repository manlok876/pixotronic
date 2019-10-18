// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "GameFramework/Pawn.h"
#include "PT_BaseBike.generated.h"

UCLASS(Blueprintable)
class PIXOTRONIC_API APT_BaseBike : public APawn
{
	GENERATED_BODY()

public:
	APT_BaseBike();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Determines how fast the bike moves
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

	// Number of times that special ability can be used
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AbilityPoints;

	// Color used for bike and its trail
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor PlayerColor;

	UFUNCTION()
	void TurnLeft();

	UFUNCTION()
	void TurnRight();

	UFUNCTION()
	void OnCollide(UPrimitiveComponent* OverlappedComponent, 
				   AActor* OtherActor, UPrimitiveComponent* OtherComp, 
				   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	static const FName TurnLeftBinding;
	static const FName TurnRightBinding;
	static const FName UseAbilityBinding;

	static const FVector BikeMeshOffset;

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	class UPT_TrailingComponent* TrailingComponent;
};
