// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MenuInterface.h"
#include "MainMenu.generated.h"



USTRUCT()
struct FServerData
{
	GENERATED_USTRUCT_BODY()

	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUserName;

};

/**
 * 
 */
UCLASS()
class NETWORKA_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);

public:
	void SetMenuInterface(IMenuInterface* _MenuInterface);

	void Setup();
	void TearDown();	

	
	void SetServerList(TArray<FServerData> ServerNames);

	void SetSelectedIndex(uint32 Index);

	void UpdateSlots(uint32 Index);

	

private:
//MainMenu
	UPROPERTY( meta = (BindWidget) )
	class UButton* HostBtn;

	UPROPERTY( meta = (BindWidget) )
	class UButton* JoinBtn;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;
//JoinMenu
	UPROPERTY(meta = (BindWidget))
	class UButton* Join;

	UPROPERTY(meta = (BindWidget))
	class UButton* Cancel;

	UPROPERTY(meta = (BindWidget))
	class UWidget*  JoinMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ServerList;

	// Host Menu
	UPROPERTY(meta = (BindWidget))
	class UWidget* HostMenu;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* HostName;

	UPROPERTY(meta = (BindWidget))
	class UButton* 	CreateBtn_Host;

	UPROPERTY(meta = (BindWidget))
	class UButton* 	CancelBtn_Host;
	//






	//MenuSwitcher

private:
	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void ShowJoinMenu();

	UFUNCTION()
	void OnCancel();
	
	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void ShowHostMenu();



	IMenuInterface* MenuInterface;

	TSubclassOf<UUserWidget> ServerRow;

	TOptional<uint32> SelectedIndex;

public:
	virtual bool Initialize() override;

protected:
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

};
