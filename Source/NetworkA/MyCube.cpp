// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCube.h"




AMyCube::AMyCube()
{
	PrimaryActorTick.bCanEverTick = true;

	SetMobility(EComponentMobility::Movable);	
}

void AMyCube::BeginPlay()
{
	Super::BeginPlay();
	// if This Code Excute on Server..
	if (HasAuthority())
	{
		
		SetReplicates(true);
		SetReplicateMovement(true);
	}

	
	GlobalTargetLoc = GetTransform().TransformPosition(TargetPos);
	GlobalTurrnLoc	= GetTransform().TransformPosition(TurningPos);
	
	
}

void AMyCube::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	static int t = 0;

	
	//Excute on Server..
	if (HasAuthority()) 
	{
		auto Loc = GetActorLocation();

		//To TargetPos..
		
		FVector Dir = (GlobalTargetLoc - GlobalTurrnLoc).GetSafeNormal();
		Loc += Speed * DeltaSeconds * Dir;
		SetActorLocation(Loc);

		//and too Much Near of TargetPos
		float TurnLength = (Loc - GlobalTurrnLoc).Size();
		float TargetToTargetLength = (GlobalTargetLoc - GlobalTurrnLoc).Size();

		//터닝->타겟보다 터닝-> Loc의 값이 더커지면 스왑

		if (TurnLength >= TargetToTargetLength)
		{			
	
			FVector Temp = GlobalTargetLoc;
			GlobalTargetLoc = GlobalTurrnLoc;
			GlobalTurrnLoc = Temp;
		}
		

		
	}
	


}
