// Fill out your copyright notice in the Description page of Project Settings.


#include "PT_ArenaGameMode.h"

#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

#include "PT_BaseBike.h"
#include "PT_ArenaPlayerController.h"
#include "PT_ArenaGameState.h"

APT_ArenaGameMode::APT_ArenaGameMode() 
{
	DefaultPawnClass = APT_BaseBike::StaticClass();
	PlayerControllerClass = APT_ArenaPlayerController::StaticClass();
	GameStateClass = APT_ArenaGameState::StaticClass();
}

AActor* APT_ArenaGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	for (TActorIterator<APlayerStart> StartItr(GetWorld()); StartItr; ++StartItr)
	{
		break;
	}
	return Super::ChoosePlayerStart_Implementation(Player);
}
