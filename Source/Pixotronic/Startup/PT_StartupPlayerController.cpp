// Copyright 2019 Denis Maximenko


#include "PT_StartupPlayerController.h"

void APT_StartupPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Return", IE_Pressed, this, &APT_StartupPlayerController::HandleBackPressed);
}

void APT_StartupPlayerController::HandleBackPressed()
{
	OnBackButtonPressed.Broadcast();
}
