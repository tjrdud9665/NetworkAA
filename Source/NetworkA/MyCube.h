// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MyCube.generated.h"

/**
 * 
 */

using namespace UP;

UCLASS()
class NETWORKA_API AMyCube : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	AMyCube();
	
		
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement" , meta = (MakeEditWidget))
	FVector TargetPos = FVector(0, 0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (MakeEditWidget))
	FVector TurningPos = FVector(0, 0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float Speed = 100;

private:	

	FVector GlobalTargetLoc;

	FVector GlobalTurrnLoc;

	

	


	
};
