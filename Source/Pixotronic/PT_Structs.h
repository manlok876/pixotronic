// Copyright 2019 Denis Maximenko

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Arena/PT_AbilityInterface.h"
#include "PT_Structs.generated.h"

USTRUCT(BlueprintType)
struct FPT_BikeModel
{
	GENERATED_USTRUCT_BODY()

	/** used to display info about selected model. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	/** Mesh to use for this bike. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* Mesh;

	/** Class of component responsible for special ability of this bike */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MustImplement="PT_AbilityInterface"))
	TSubclassOf<UActorComponent> AbilityComponentClass;

	FPT_BikeModel();
};
