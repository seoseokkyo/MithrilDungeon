// Fill out your copyright notice in the Description page of Project Settings.


#include "Potion.h"

APotion::APotion()
{
	DesiredItemID = FName(TEXT("test_001"));
}

void APotion::BeginPlay()
{
	Super::BeginPlay();
}
