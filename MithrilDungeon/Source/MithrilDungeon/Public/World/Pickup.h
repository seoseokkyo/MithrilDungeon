// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class MITHRILDUNGEON_API APickup : public AActor//, public IInteractionInterface;
{
	GENERATED_BODY()
	
public:	
	//================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================




	//================================================================================
	// FUNCTIONS
	//================================================================================
	APickup();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


	//================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================




	//================================================================================
	// FUNCTIONS
	//================================================================================
};
