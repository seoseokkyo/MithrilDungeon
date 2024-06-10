// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/Pickup.h"
#include "Potion.generated.h"

/**
 * 
 */
UCLASS()
class MITHRILDUNGEON_API APotion : public APickup
{
	GENERATED_BODY()
	
public:
	APotion();

	virtual void BeginPlay() override;
	//virtual void Tick(float DeltaTime) override;

};
