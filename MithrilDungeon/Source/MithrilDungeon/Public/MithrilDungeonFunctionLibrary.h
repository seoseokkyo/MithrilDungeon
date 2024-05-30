// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MithrilDungeonFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MITHRILDUNGEON_API UMithrilDungeonFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	static UTexture2D* GetImageByItemName(UObject* WorldContextObject, const FString& rowName);
};
