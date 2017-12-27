// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "ServerRow.h"
#include "ConstructorHelpers.h"


UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	//TODO : Get ServerRow Widet Blueprint's Class and Send to Private Member ClassInfo.
	ConstructorHelpers::FClassFinder<UServerRow> ServerRowWIdget(TEXT("/Game/Widget/W_ServerRow"));


	if (ensure(ServerRowWIdget.Class))
	{
		ServerRow = ServerRowWIdget.Class;		

	}
	


}

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success)
		return false;

	//	TODO : Setup.


	
	if (!ensure(HostBtn))
		return false;
	HostBtn->OnClicked.AddDynamic(this, &UMainMenu::ShowHostMenu);

	//For .HostMenu
	CreateBtn_Host->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	CancelBtn_Host->OnClicked.AddDynamic(this, &UMainMenu::OnCancel);

	

	if (!ensure(JoinBtn))
		return false;
	//For JoinMenu..
	JoinBtn->OnClicked.AddDynamic(this, &UMainMenu::ShowJoinMenu);

	Join->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
	Cancel->OnClicked.AddDynamic(this, &UMainMenu::OnCancel);	
	


	return true;
}

void UMainMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);

	TearDown();

	UE_LOG(LogTemp, Warning, TEXT("LEVEL End "));
}



void UMainMenu::SetMenuInterface(IMenuInterface* _MenuInterface)
{
	MenuInterface = _MenuInterface;
}

void UMainMenu::Setup()
{
	this->AddToViewport();

	//SetInput Mode.
	UWorld* World = GetWorld();
	if(!ensure(World))
		return;
	


	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController))
		return;


	FInputModeUIOnly UIOnlyInputMode;

	UIOnlyInputMode.SetWidgetToFocus(this->TakeWidget());
	UIOnlyInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);

	PlayerController->SetInputMode(UIOnlyInputMode);
	PlayerController->bShowMouseCursor = true;

	
}

void UMainMenu::TearDown()
{
	this->RemoveFromViewport();
	//SetInput Mode.
	UWorld* World = GetWorld();
	if (!ensure(World))
		return;



	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController))
		return;

	FInputModeGameOnly GameOnly;	
	

	PlayerController->SetInputMode(GameOnly);
	PlayerController->bShowMouseCursor = false;
}


void UMainMenu::HostServer()
{
	if (MenuInterface)
	{
		//TODO : Get HostName Text..
		//TODO : Make and Send CustomSessionData..		
		MenuInterface->Host(HostName->GetText().ToString());
	}

}

void UMainMenu::ShowJoinMenu()
{
	if (!ensure(MenuSwitcher))
		return;

	MenuSwitcher->SetActiveWidget(JoinMenu);

	if (MenuInterface)
		MenuInterface->RefreshServerList();

}

void UMainMenu::ShowHostMenu()
{
	if (!ensure(MenuSwitcher))
		return;

	MenuSwitcher->SetActiveWidget(HostMenu);
}



/////////////////////////////////////////////////////////////////////////JoinMenu
void UMainMenu::SetServerList(TArray<FServerData> ServerNames)
{

	//TODO : Clear Previous List..
	ServerList->ClearChildren();
	//TODO : Make ServerList
	uint32 i = 0;



	for (const FServerData& ServerData : ServerNames)
	{
		//TODO : Create ServerRow Widget..
		UWorld* World = GetWorld();
		if (!ensure(World))
			return;
		UServerRow* Row = CreateWidget<UServerRow>(World, ServerRow);
		//TODO : Add to ServerRow in ServerList..
		if (!ensure(Row))
		{
			Row->Destruct();
			return;
		}			

		Row->SettingUp(ServerData);
		Row->SetupParent(this, i++);
		ServerList->AddChild(Row);
	}
	


}

void UMainMenu::SetSelectedIndex(uint32 Index)
{
	SelectedIndex = Index;	
	UServerRow* Row = Cast<UServerRow>(ServerList->GetChildAt(Index));	
	Row->Selected = true;

}

void UMainMenu::UpdateSlots(uint32 Index)
{	
	for (int32 i = 0; i < ServerList->GetChildrenCount(); i++)
	{
		UServerRow* Row = Cast<UServerRow>(ServerList->GetChildAt(i));
		Row->Selected = false;		
	}
	SetSelectedIndex(Index);
}


void UMainMenu::OnCancel()
{
	if (!ensure(MenuSwitcher))
		return;

	MenuSwitcher->SetActiveWidget(MainMenu);

}

void UMainMenu::JoinServer()
{
	if (SelectedIndex.IsSet())
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected is %d"), SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Do not Selected!!"));
	}

	if (MenuInterface && SelectedIndex.IsSet())
	{
		MenuInterface->Join(SelectedIndex.GetValue());
	}

}

