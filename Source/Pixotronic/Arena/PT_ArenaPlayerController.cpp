// Fill out your copyright notice in the Description page of Project Settings.


#include "PT_ArenaPlayerController.h"

#include "EngineUtils.h"
#include "Camera/CameraActor.h"

APT_ArenaPlayerController::APT_ArenaPlayerController()
{
	bAutoManageActiveCameraTarget = false;
}

void APT_ArenaPlayerController::BeginPlay()
{
	for (TActorIterator<ACameraActor> CameraItr(GetWorld()); CameraItr; ++CameraItr)
	{
		SetViewTarget(*CameraItr);
		break;
	}
}