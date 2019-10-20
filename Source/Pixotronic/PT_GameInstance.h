// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "PT_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PIXOTRONIC_API UPT_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPT_GameInstance();

	TSharedPtr<class FOnlineSessionSettings> SessionSettings;

	// Hosting a session

	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);

	bool HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);

	// Finding a session

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;
	void OnFindSessionsComplete(bool bWasSuccessful);

	void FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence);

private:
	static const FName ArenaMapName;
};
