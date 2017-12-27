// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuInterface.generated.h"

#define PURE = 0

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMenuInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class NETWORKA_API IMenuInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Host(const FString& HostName)	PURE;
	virtual void Join(uint32 index)				PURE;

	virtual void RefreshServerList()			PURE;
	
	//virtual class UNetworkInstance* GetInst
	
};
