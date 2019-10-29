// Copyright 2019 Denis Maximenko

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "CreateSessionCallbackProxy.h"
#include "FindSessionsCallbackProxy.h"
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

	UFUNCTION(BlueprintCallable)
	void StartOnlineGame();

	UPROPERTY(BlueprintAssignable)
	FEmptyOnlineDelegate OnSessionCreateFinishedDelegate;

	// Finding a session

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
	TArray<FBlueprintSessionResult> BP_SearchResults;

	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;
	void OnFindSessionsComplete(bool bWasSuccessful);

	void FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence);

	UFUNCTION(BlueprintCallable)
	void FindOnlineSessions();

	UPROPERTY(BlueprintAssignable)
	FBlueprintFindSessionsResultDelegate OnSessionSearchFinishedDelegate;

	// Joining a session

	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	virtual bool JoinSession(ULocalPlayer* LocalPlayer, const FOnlineSessionSearchResult& SearchResult) override;

	UFUNCTION(BlueprintCallable)
	void JoinOnlineSession(const FBlueprintSessionResult& Session);

	// Destroying session

	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;
	virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void DestroySessionAndLeave();

private:
	static const FName ArenaMapName;
	static const FName StartupMapName;
};
