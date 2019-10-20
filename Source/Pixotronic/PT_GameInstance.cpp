// Fill out your copyright notice in the Description page of Project Settings.


#include "PT_GameInstance.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Kismet/GameplayStatics.h"

const FName UPT_GameInstance::ArenaMapName("PT_Arena");

UPT_GameInstance::UPT_GameInstance()
{
	OnCreateSessionCompleteDelegate =
		FOnCreateSessionCompleteDelegate::CreateUObject(this, &UPT_GameInstance::OnCreateSessionComplete);

	OnStartSessionCompleteDelegate =
		FOnStartSessionCompleteDelegate::CreateUObject(this, &UPT_GameInstance::OnStartSessionComplete);
}

bool UPT_GameInstance::HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers)
{
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr SessionInterface = OnlineSub->GetSessionInterface();

		if (SessionInterface.IsValid() && UserId.IsValid())
		{
			SessionSettings = MakeShareable(new FOnlineSessionSettings());

			SessionSettings->bIsLANMatch = bIsLAN;
			SessionSettings->bUsesPresence = bIsPresence;
			SessionSettings->NumPublicConnections = MaxNumPlayers;
			SessionSettings->NumPrivateConnections = 0;
			SessionSettings->bAllowInvites = true;
			SessionSettings->bAllowJoinInProgress = false;
			SessionSettings->bShouldAdvertise = true;
			SessionSettings->bAllowJoinViaPresence = true;
			SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;

			SessionSettings->Set(SETTING_MAPNAME, ArenaMapName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

			OnCreateSessionCompleteDelegateHandle =
				SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

			return SessionInterface->CreateSession(*UserId, SessionName, *SessionSettings);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No OnlineSubsytem found!"));
	}

	return false;
}

void UPT_GameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr SessionInterface = OnlineSub->GetSessionInterface();

		if (SessionInterface.IsValid())
		{
			SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
			if (bWasSuccessful)
			{
				OnStartSessionCompleteDelegateHandle =
					SessionInterface->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);

				SessionInterface->StartSession(SessionName);
			}
		}
	}
}

void UPT_GameInstance::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		}
	}

	if (bWasSuccessful)
	{
		UGameplayStatics::OpenLevel(this, ArenaMapName, true, "listen");
	}
}
