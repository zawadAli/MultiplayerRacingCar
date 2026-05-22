// Fill out your copyright notice in the Description page of Project Settings.


#include "StartMenu.h"
#include "Components/Button.h"
#include "MultiplayerRacingCar/Multiplayer/Subsystem/MultiplayerSessionsSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Components/ComboBoxString.h"
#include "Components/CheckBox.h"
#include "Components/EditableTextBox.h"
#include "Interfaces/OnlineSessionInterface.h"

void UStartMenu::StartMenu()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	SetIsFocusable(true);

	if(UWorld * World = GetWorld()) {
		if (APlayerController* PlayerController = World->GetFirstPlayerController()) {
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputMode);
			PlayerController->SetShowMouseCursor(true);
		}
	}

	if (UGameInstance* GameInstance = GetGameInstance()) {
		SessionSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		if (SessionSubsystem) {
			SessionSubsystem->MultiplayerCreateSessionCompleteDelegate.AddDynamic(this, &UStartMenu::OnMultiplayerCreateSession);
			SessionSubsystem->MultiplayerFindSessionsCompleteDelegate.AddUObject(this, &UStartMenu::OnMultiplayerFindSessionsComplete);
			SessionSubsystem->MultiplayerJoinSessionCompleteDelegate.AddUObject(this, &UStartMenu::OnMultiplayerJoinSessionComplete);
		}
	}

}

bool UStartMenu::Initialize()
{
	if (!Super::Initialize())
		return false;
	
	if (StartGame)
		StartGame->OnClicked.AddDynamic(this, &UStartMenu::StartGameClicked);
	if (JoinGame)
		JoinGame->OnClicked.AddDynamic(this, &UStartMenu::JoinGameClicked);
	if (bCustomSessionID)
		bCustomSessionID->OnCheckStateChanged.AddDynamic(this, &UStartMenu::OnCustomSessionClicked);
	return true;
}

void UStartMenu::StartGameClicked()
{
	if (SessionSubsystem)
	{
		int32 NumbPlayers = FCString::Atoi(*NumberOfPlayers->GetSelectedOption());
		SessionSubsystem->CreateSession(NumbPlayers , MatchTypeName);
		if (StartGame) {
			StartGame->SetIsEnabled(false);
		}
	}
}

void UStartMenu::JoinGameClicked()
{
	if (SessionSubsystem)
	{
		SessionSubsystem->FindSessions();
		if (JoinGame) {
			JoinGame->SetIsEnabled(false);
		}
	}
}

void UStartMenu::OnCustomSessionClicked(bool bIsChecked)
{
	NewSessionID->SetIsReadOnly(!bIsChecked);
}

void UStartMenu::Remove_Menu()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, TEXT("Remove Menu call"));
	}
	RemoveFromParent();
	if (UWorld* World = GetWorld()) {
		if (APlayerController* PlayerController = World->GetFirstPlayerController()) {
			FInputModeGameOnly InputMode;
			PlayerController->SetInputMode(InputMode);
			PlayerController->SetShowMouseCursor(false);
		}
	}
}

void UStartMenu::OnMultiplayerCreateSession(bool bWasSuccessful)
{
	if (StartGame) {
		StartGame->SetIsEnabled(true);
	}
	if (bWasSuccessful)
	{
		Remove_Menu();
		if (UWorld* World = GetWorld()) {
			World->ServerTravel(MapLocation);
		}
	}
}

void UStartMenu::OnMultiplayerFindSessionsComplete(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
	if (!SessionSubsystem) return;
	if (bWasSuccessful) {
		for (auto Result : SessionResults)
		{
			FString MatchType;
			Result.Session.SessionSettings.Get(FName("MatchType"), MatchType);

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1,
					10.f,
					FColor::Cyan,
					FString::Printf(TEXT("Found MatchType: %s"), *MatchType)
				);
			}
			if (bCustomSessionID->IsChecked()) {
				if (MatchType == (MatchTypeName + NewSessionID->GetText().ToString())) {
					SessionSubsystem->JoinSession(Result);
					return;
				}
			}
			else {
				if (MatchType.StartsWith(FString("ProMultiplayerGame"), ESearchCase::CaseSensitive))
				{
					SessionSubsystem->JoinSession(Result);
					return;
				}
			}
	
		}
	}
	if (JoinGame) {
		JoinGame->SetIsEnabled(true);
	}
}

void UStartMenu::OnMultiplayerJoinSessionComplete(EOnJoinSessionCompleteResult::Type Result)
{
	if (JoinGame) {
		JoinGame->SetIsEnabled(true);
	}
	if (IOnlineSubsystem* OnlineSubsystemPtr = IOnlineSubsystem::Get()) {
		if (IOnlineSessionPtr SessionInterface = OnlineSubsystemPtr->GetSessionInterface())
		{
			FString MatchAddress;
			const bool bGotAddress = SessionInterface->GetResolvedConnectString(NAME_GameSession, MatchAddress);

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1,
					50.f,
					bGotAddress ? FColor::Green : FColor::Red,
					FString::Printf(TEXT("Match Address: %s"), *MatchAddress)
				);
			}

			if (bGotAddress && !MatchAddress.IsEmpty())
			{
				if (APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController())
				{
					Remove_Menu();
					PlayerController->ClientTravel(MatchAddress, ETravelType::TRAVEL_Absolute);
				}
			}
		}
	}
	
}
