// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetworkAGameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKA_API ALobbyGameMode : public ANetworkAGameMode
{
	GENERATED_BODY()
	
public:
	void StartGame();	
	
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

public:
	UFUNCTION(BlueprintCallable)
		void GetAllController(TArray<APlayerController*>& AllControllers);
private:
	uint32 PlayerCount = 0;

	FTimerHandle GameStartTimer;


};
