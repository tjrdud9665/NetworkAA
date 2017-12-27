// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerRow.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "MainMenu.h"





bool UServerRow::Initialize()
{
	Super::Initialize();

	//TODO : Bind Delegate OnClickedJoinBtn..
	//TODO : Set Text Server Name
		
	//ServerName->SetText()
	ServerRowBtn->OnClicked.AddDynamic(this, &UServerRow::OnClickedServerRowBtn);


	return true;
}

void UServerRow::SetupParent(class UMainMenu* Parent, uint32 Index)
{
	Menu = Parent;
	this->Index = Index;
}

void UServerRow::SettingUp(const FServerData& Data)
{
	bHeader = false;
	ServerName->SetText(FText::FromString(Data.Name));
	ConnectionFraction->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), Data.CurrentPlayers, Data.MaxPlayers)));
	HostUserName->SetText(FText::FromString(Data.HostUserName));
}

void UServerRow::OnClickedServerRowBtn()
{
	if (!ensure(Menu))
	{
		UE_LOG(LogTemp, Error, TEXT("Parent is NULL"));
		return;
	}	
	Menu->UpdateSlots(Index);	
	UE_LOG(LogTemp, Warning, TEXT("Selected Value : %d") , Index);
	
}

void UServerRow::NativeConstruct()
{
	if (bHeader)
	{
		ServerRowBtn->SetIsEnabled(false);
		HeaderContent->SetVisibility(ESlateVisibility::Visible);
		BtnContent->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		HeaderContent->SetVisibility(ESlateVisibility::Hidden);
		BtnContent->SetVisibility(ESlateVisibility::Visible);
	}
}
