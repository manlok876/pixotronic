// Fill out your copyright notice in the Description page of Project Settings.


#include "PT_StartupGameMode.h"
#include "PT_StartupPlayerController.h"
#include "Kismet/GameplayStatics.h"

const FName APT_StartupGameMode::ArenaLevel("PT_Arena");

APT_StartupGameMode::APT_StartupGameMode()
{
	PlayerControllerClass = APT_StartupPlayerController::StaticClass();
}

void APT_StartupGameMode::SinglePlay()
{
	UGameplayStatics::OpenLevel(this, ArenaLevel);
}

void APT_StartupGameMode::HostGame()
{
}

void APT_StartupGameMode::JoinGame()
{
}
