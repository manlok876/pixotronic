// Copyright 2019 Denis Maximenko

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PT_Structs.generated.h"

USTRUCT(BlueprintType)
struct FPT_BikeModel
{
	GENERATED_USTRUCT_BODY()

	/** used to display info about selected model. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	/** Mesh to use for this bike. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* Mesh;

	/** Class of component responsible for special ability of this bike */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UActorComponent> AbilityComponentClass;

  FPT_BikeModel();
};
