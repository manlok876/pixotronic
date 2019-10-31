// Copyright 2019 Denis Maximenko


#include "PT_ArenaPlayerController.h"

#include "EngineUtils.h"
#include "Camera/CameraActor.h"

#include "PT_GameInstance.h"
#include "PT_ArenaPlayerState.h"

const FName APT_ArenaPlayerController::ReturnBinding("Return");

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

void APT_ArenaPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(ReturnBinding, IE_Released, this, &APT_ArenaPlayerController::LeaveGame);
}

void APT_ArenaPlayerController::NotifyReadiness_Implementation(bool IsReady)
{
	APT_ArenaPlayerState* MyState = CastChecked<APT_ArenaPlayerState>(PlayerState);
	MyState->ReadyToStart = IsReady;
}

bool APT_ArenaPlayerController::NotifyReadiness_Validate(bool IsReady)
{
	return true;
}

void APT_ArenaPlayerController::LeaveGame()
{
	UPT_GameInstance* GameInstance = GetGameInstance<UPT_GameInstance>();
	if (GameInstance != nullptr)
	{
		GameInstance->DestroySessionAndLeave();
	}
}
