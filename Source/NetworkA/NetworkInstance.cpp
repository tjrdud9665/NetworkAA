// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkInstance.h"
#include "Engine/Engine.h"
#include "OnlineSubsystem.h"
#include "UObject/ConstructorHelpers.h"
#include "MenuSystem/MainMenu.h"
#include "OnlineSessionSettings.h"



const static FName SESSION_NAME = TEXT("GameSession");

//"D:\UE_4.18\Engine\Binaries\Win64\UE4Editor.exe" "D:\PrivateGame\NetworkA\NetworkA.uproject" - game - log - nosteam


UNetworkInstance::UNetworkInstance(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UMainMenu> MainMenuWidet(TEXT("/Game/Widget/W_MainMennu"));
	if (!ensure(MainMenuWidet.Class))
		return;

	MenuClass = MainMenuWidet.Class;	
	
	UE_LOG(LogTemp, Warning, TEXT("Constructor Call!"));
	
}

void UNetworkInstance::LoadMenuWidget()
{
	if (!ensure(MenuClass))
	{
		UE_LOG(LogTemp, Error, TEXT("WidgetClass is NULL"));
		return;
	}

	Menu = CreateWidget<UMainMenu>(this, MenuClass);

	if (!ensure(Menu))
	{
		UE_LOG(LogTemp, Error, TEXT("Widegt Instance is NULL"));
		return;
	}


	Menu->Setup();
	Menu->SetMenuInterface(this);	

	

			
}

void UNetworkInstance::Host(const FString& HostName)
{	
	
	if (SessionInterface.IsValid())
	{
		if(bCurrentSessionFinding)
			SessionInterface->CancelFindSessions();

		auto ExistingSession =  SessionInterface->GetNamedSession(SESSION_NAME);

		if (ExistingSession != nullptr)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			this->CreateSession(HostName);
		}


	}

	
}

void UNetworkInstance::Join(uint32 index)
{
	//TODO : Join Session...

	if (!SessionInterface.IsValid())
		return;
	
	if (!SessionSearch.IsValid())
		return;

	auto JoinableSessions = SessionSearch->SearchResults;
	if (JoinableSessions.IsValidIndex(index))
	{
		SessionInterface->JoinSession(0, SESSION_NAME, JoinableSessions[index]);	

	}	

	

}

void UNetworkInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("InitInstance"));

	IOnlineSubsystem* SubSystem =  IOnlineSubsystem::Get();

	if(SubSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("SubSystem : %s"), *SubSystem->GetSubsystemName().ToString());
		SessionInterface = SubSystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UNetworkInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UNetworkInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UNetworkInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UNetworkInstance::OnJoinSessionComplete);

			RefreshServerList();			

		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No SubSystem"));
	}


}

void UNetworkInstance::StartSession()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->StartSession(SESSION_NAME);
	}
}

void UNetworkInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{

	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't Creaet Session"));
		return;
	}


	UEngine* Engine = GetEngine();
	if (!ensure(Engine))
		return;

	Engine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("Command Test Host"));
	UWorld* World = GetWorld();

	if (!ensure(World))
		return;


	World->ServerTravel("/Game/Lobby?listen");

}

void UNetworkInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	//if (Success)
	//{
	//	FString HostName;
	//	SessionInterface->GetSessionSettings(SESSION_NAME)->Get(FName(TEXT("HostName")), HostName);
	//	CreateSession(HostName);

	//}
	
}

void UNetworkInstance::OnFindSessionsComplete(bool Success)
{
	UE_LOG(LogTemp, Warning, TEXT("SessionFind Complete!"));
	bCurrentSessionFinding = false;

	if (Success && SessionSearch.IsValid() && Menu != nullptr)
	{
		if (SessionSearch->SearchResults.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("didn't Found Session"));
			//return;
		}
		TArray<FServerData> ServerDatas;	
		//be use Const because SessionResult do not want Change Result Data
		for (const FOnlineSessionSearchResult& Result : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Founded Session  : %s "), *Result.GetSessionIdStr());
			FServerData Data;
			Data.Name = Result.GetSessionIdStr();			
			Data.MaxPlayers = Result.Session.SessionSettings.NumPublicConnections;

			Data.CurrentPlayers = Data.MaxPlayers -  Result.Session.NumOpenPublicConnections;

			
			Result.Session.SessionSettings.Get(FName(TEXT("HostName")) , Data.HostUserName);

			ServerDatas.Add(Data);	
			
		}

		Menu->SetServerList(ServerDatas);

	}

	
}

void UNetworkInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{

	if (!SessionInterface.IsValid())
		return;
	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SESSION_NAME, Address))
	{
		UE_LOG(LogTemp, Error, TEXT("Session Invalid"));
		return;
	}
		


	UEngine* Engine = GetEngine();
	if (!ensure(Engine)) return;

	Engine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("Joining %s"),*Address ) );


	UWorld* World = GetWorld();

	if (!ensure(World))
	{
		UE_LOG(LogTemp, Error, TEXT("World is Invalid"));
		return;
	}
		

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (!ensure(PlayerController))
	{
		UE_LOG(LogTemp, Error, TEXT("Controller is Invalid"));
		return;
	}		

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UNetworkInstance::CreateSession(const FString& HostName)
{
	if (SessionInterface.IsValid())
	{

		FOnlineSessionSettings SessionSettings;

		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
		{
			SessionSettings.bIsLANMatch = true;
		}
		else
		{
			SessionSettings.bIsLANMatch = false;			
		}
		
		SessionSettings.bUsesPresence = true;
		SessionSettings.NumPublicConnections = 5;
		SessionSettings.bShouldAdvertise = true;		

		//Custom SessionData  Settings.
		SessionSettings.Set(FName(TEXT("HostName")), HostName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}

	
}



void UNetworkInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	if (SessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Start Find Session !"));
		

		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
		{
			SessionSearch->bIsLanQuery = true;			
		}
		else
		{
			SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
			SessionSearch->MaxSearchResults = 100;
		}



		if(bCurrentSessionFinding)
			SessionInterface->CancelFindSessions();

		bCurrentSessionFinding = true;
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SessionSearch Is Invalid"));
	}
}




	
	