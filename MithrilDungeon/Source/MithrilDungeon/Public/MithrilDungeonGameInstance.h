// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "MithrilDungeonGameInstance.generated.h"

/**
 * 
 */

 class UDataTable;

UCLASS()
class MITHRILDUNGEON_API UMithrilDungeonGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category="MySettings")
	UDataTable* dt_characerStatDataTable;

public:
	UFUNCTION(BlueprintCallable)
	FCharacterStat GetCharacterDataTable(const FString& rowName);
};
