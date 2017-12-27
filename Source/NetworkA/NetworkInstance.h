// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "OnlineSessionInterface.h"
#include "NetworkInstance.generated.h"

class UUserWidget;
/**
 * 
 */
UCLASS()
class NETWORKA_API UNetworkInstance : public UGameInstance , public IMenuInterface
{
	GENERATED_BODY()	
	

public:	
	UNetworkInstance(const FObjectInitializer& ObjectInitializer);
//Commands...
	UFUNCTION(Exec)
	virtual void Host(const FString& HostName)	override;

	UFUNCTION(Exec)
	virtual void Join(uint32 index)				override;	

	virtual void RefreshServerList()			override;

//Blueprint..
	UFUNCTION(BlueprintCallable)
	void LoadMenuWidget();

public:
	virtual void Init() override;

private:	
	//For Session
	IOnlineSessionPtr SessionInterface;
	//For Find Session..
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	bool bCurrentSessionFinding = false;

private:
	//For UI
	TSubclassOf<UUserWidget> MenuClass;

	class UMainMenu* Menu;

	//ForTimer
	FTimerHandle WaitTimer;
	

public:
	void StartSession();

private:
	//Delegates..
	void OnCreateSessionComplete(FName SessionName, bool Success);

	void OnDestroySessionComplete(FName SessionName, bool Success);

	void OnFindSessionsComplete(bool Success);

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void CreateSession(const FString& HostName);

	//For timer Delegate..



};
