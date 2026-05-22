// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "Interfaces/OnlineIdentityInterface.h"

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem() :
	OnCreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete)),
	OnFindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsComplete)),
	OnJoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete))
{
	OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		SessionInterface = OnlineSubsystem->GetSessionInterface();

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, OnlineSubsystem->GetSubsystemName().ToString());
		}
	}

}
void UMultiplayerSessionsSubsystem::CreateSession(int32 NumPlayers , FString MatchType)
{
	CurrentSessionUniqueName = MatchType + GenerateUniqueSessionId();

	if (!SessionInterface.IsValid())
	{
		MultiplayerCreateSessionCompleteDelegate.Broadcast(false);
		return;
	}
	auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
	if (ExistingSession != nullptr)
		SessionInterface->DestroySession(NAME_GameSession);

	OnCreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

	TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());

	// ADD THESE THREE LINES:
	SessionSettings->bIsLANMatch = false;
	SessionSettings->bUsesPresence = true;      // CRITICAL for Steam
	SessionSettings->bShouldAdvertise = true;   // Allows others to find it
	SessionSettings->NumPublicConnections = NumPlayers;
	SessionSettings->bUseLobbiesIfAvailable = true; // New for UE5.4+
	SessionSettings->bAllowJoinInProgress = true;
	SessionSettings->bAllowJoinViaPresence = true;
	SessionSettings->Set(FName("MatchType"), CurrentSessionUniqueName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	CurrentSessionUniqueName.RemoveFromStart(MatchType);
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *SessionSettings)) {
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
		MultiplayerCreateSessionCompleteDelegate.Broadcast(false);
	}
}
void UMultiplayerSessionsSubsystem::FindSessions()
{
	if (!SessionInterface.IsValid()) {
		MultiplayerFindSessionsCompleteDelegate.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}
	OnFindSessionCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!LocalPlayer) return;
	SearchSettings = MakeShareable(new FOnlineSessionSearch());
	SearchSettings->MaxSearchResults = 10000;
	SearchSettings->bIsLanQuery = false;
	SearchSettings->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	SearchSettings->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
	if (!SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SearchSettings.ToSharedRef())) {
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionCompleteDelegateHandle);
		MultiplayerFindSessionsCompleteDelegate.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
	}
}
void UMultiplayerSessionsSubsystem::JoinSession(const FOnlineSessionSearchResult& SessionSearchResult)
{
	FOnlineSessionSearchResult Result = SessionSearchResult;
	Result.Session.SessionSettings.bUseLobbiesIfAvailable = true;
	Result.Session.SessionSettings.bUsesPresence = true;

	OnJoinSessionCompleteDelegateHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!LocalPlayer) return;

	if (!SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, Result))
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);
		MultiplayerJoinSessionCompleteDelegate.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	}

	
}
void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if(SessionInterface.IsValid())
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
	MultiplayerCreateSessionCompleteDelegate.Broadcast(bWasSuccessful);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			40.f,
			FColor::Green,
			FString::Printf(TEXT("Session Name: %s"), *CurrentSessionUniqueName)
		);
	}

}

//void UMultiplayerSessionsSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
//{
//	if (!SessionInterface.IsValid()) return;
//	if (bWasSuccessful) {
//		for (auto Result : SearchSettings->SearchResults) {
//			FString MatchType;
//			Result.Session.SessionSettings.Get(FName("MatchType"), MatchType);
//			if (MatchType ==FString("ProMultiplayerGame")) {
//				//Result.Session.SessionSettings.bUseLobbiesIfAvailable = true;
//				//Result.Session.SessionSettings.bUsesPresence = true;
//				SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
//				const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
//				SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, Result);
//				return;
//			}
//		}
//	}
//}
void UMultiplayerSessionsSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (!SessionInterface.IsValid()) {
		MultiplayerFindSessionsCompleteDelegate.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}
	SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionCompleteDelegateHandle);
	if (!bWasSuccessful || !SearchSettings.IsValid())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("FindSessions failed"));
		}
		return;
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			10.f,
			FColor::Yellow,
			FString::Printf(TEXT("Sessions Found: %d"), SearchSettings->SearchResults.Num())
		);
	}
	if (SearchSettings->SearchResults.Num() <= 0) {
		MultiplayerFindSessionsCompleteDelegate.Broadcast(TArray<FOnlineSessionSearchResult>(), bWasSuccessful);
		return;
	}
	MultiplayerFindSessionsCompleteDelegate.Broadcast(SearchSettings->SearchResults, bWasSuccessful);


}
//void UMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) {
//	if (!SessionInterface.IsValid()) return;
//	FString MatchAddress;
//	const bool bGotAddress = SessionInterface->GetResolvedConnectString(NAME_GameSession, MatchAddress);
//
//	if (GEngine)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Green, FString::Printf(TEXT("Match Address: %s"), *MatchAddress));
//	}
//
//	if (bGotAddress)
//	{
//		if (APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController())
//		{
//			PlayerController->ClientTravel(MatchAddress, ETravelType::TRAVEL_Absolute);
//		}
//	}
//}

void UMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

	}
	MultiplayerJoinSessionCompleteDelegate.Broadcast(Result);


}

FString UMultiplayerSessionsSubsystem::GenerateUniqueSessionId() const
{
	/// player Name + _ + time now
	FString PlayerSteamName = FString("");
	if (IOnlineSubsystem* OnlineSubsystemSteam = IOnlineSubsystem::Get(STEAM_SUBSYSTEM))
	{
		IOnlineIdentityPtr Identity = OnlineSubsystemSteam->GetIdentityInterface();
		if (Identity.IsValid())
		{
			const FUniqueNetIdPtr LocalPlayerID = Identity->GetUniquePlayerId(0);
			if (LocalPlayerID.IsValid())
				PlayerSteamName = Identity->GetPlayerNickname(*LocalPlayerID);
		}
	}
	int64 TimeStamp = FDateTime::Now().ToUnixTimestamp();
	FString TimestampString = FString::Printf(TEXT("%04d"), TimeStamp % 10000);
	return PlayerSteamName + TEXT("_") + TimestampString;
}
