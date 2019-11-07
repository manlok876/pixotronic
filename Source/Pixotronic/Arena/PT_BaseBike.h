// Copyright 2019 Denis Maximenko

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PT_BaseBike.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathDelegate, APawn*, Bike);

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
	virtual void PossessedBy(AController* NewController) override;

	// Determines how fast the bike moves
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

	// Number of times that special ability can be used
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AbilityPoints;

	// Color used for bike and its trail
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor BikeColor;

	UFUNCTION(NetMulticast, Reliable)
	void SetColor(const FLinearColor& NewColor);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void SetTurningEnabled(bool Enable);

	UFUNCTION(Server, Reliable, WithValidation)
	void TurnLeft();

	UFUNCTION(Server, Reliable, WithValidation)
	void TurnRight();

	UFUNCTION(NetMulticast, Reliable)
	void OnCollide(UPrimitiveComponent* OverlappedComponent, 
				   AActor* OtherActor, UPrimitiveComponent* OtherComp, 
				   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(BlueprintAssignable)
	FOnDeathDelegate OnDeath;

protected:
	static const FName TurnLeftBinding;
	static const FName TurnRightBinding;
	static const FName UseAbilityBinding;

	static const FVector BikeMeshOffset;

	// Specifies portion of the screen that can be used for turning (for each side)
	static const float TurningTouchArea;

	static const FName ColorParameterName;

	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleTouchInput(ETouchIndex::Type FingerIndex, FVector Location);

	UFUNCTION(NetMulticast, Reliable)
	void UpdateTransform(const FTransform& NewTransform);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	class UPT_TrailingComponent* TrailingComponent;

	UPROPERTY(Replicated)
	bool IsDead;

	UPROPERTY(Replicated)
	bool IsInputEnabled;
};
