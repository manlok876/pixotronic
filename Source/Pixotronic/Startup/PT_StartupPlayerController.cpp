// Copyright 2019 Denis Maximenko


#include "PT_StartupPlayerController.h"

APT_StartupPlayerController::APT_StartupPlayerController()
{
	ReadyToStart = false;
}

void APT_StartupPlayerController::SetReadyToStart_Implementation(bool IsReady)
{
	ReadyToStart = IsReady;
}

bool APT_StartupPlayerController::SetReadyToStart_Validate(bool IsReady)
{
	return true;
}

bool APT_StartupPlayerController::GetReadyToStart()
{
	return ReadyToStart;
}
