// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"
#include "ServerRow.generated.h"


/**
 * 
 */
UCLASS()
class NETWORKA_API UServerRow : public UUserWidget
{
	GENERATED_BODY()
	

//Constructor

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerName;
	
	UPROPERTY(BlueprintReadOnly,meta = (BindWidget))
	class UButton* ServerRowBtn;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ConnectionFraction;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HostUserName;

	UPROPERTY(meta = (BindWidget))
		class UHorizontalBox* BtnContent;

	UPROPERTY(meta = (BindWidget))
		class UHorizontalBox* HeaderContent;



	
	class UMainMenu* Menu;

	uint32 Index;

	UPROPERTY(BlueprintReadOnly)
	bool Selected = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category = "Setting" ,  meta =(ExposeOnSpawn = "true"))
	bool bHeader = true;



		
public:
	virtual bool Initialize() override;

	void SetupParent(class UMainMenu* Parent ,uint32 Index);

	void SettingUp(const FServerData& Data);


private:
	//Button Delegate Event.
	UFUNCTION()
	void OnClickedServerRowBtn();



protected:
	UFUNCTION(BlueprintCallable , Category = "Setting" )
	virtual void NativeConstruct() override;

};
