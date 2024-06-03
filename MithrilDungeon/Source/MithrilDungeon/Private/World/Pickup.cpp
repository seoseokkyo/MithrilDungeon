// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Pickup.h"

// Sets default values
APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = true;

}


void APickup::BeginPlay()
{
	Super::BeginPlay();
	
}


void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

