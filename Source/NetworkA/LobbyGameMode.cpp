// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"
#include "UnrealMath.h"
#include "TimerManager.h"
#include "NetworkInstance.h"





void ALobbyGameMode::StartGame()
{
	UWorld* World = GetWorld();

	
	
	if (ensure(World))
	{
		bUseSeamlessTravel = true;
		World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
		auto Gameinst = Cast<UNetworkInstance>(GetGameInstance());
		if(Gameinst)
		{
			Gameinst->StartSession();
		}

		
	}
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);	

	PlayerCount++;
	if (PlayerCount >= 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reached 3 Players"));
		GetWorldTimerManager().SetTimer(GameStartTimer, this, &ALobbyGameMode::StartGame, 5.0f);
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	PlayerCount--;
	PlayerCount = FMath::Clamp<uint32>(PlayerCount, 0, 100);
}

void ALobbyGameMode::GetAllController(TArray<APlayerController*>& AllControllers)
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)	
	{
		AllControllers.Add(Iterator->Get());
	}
}
