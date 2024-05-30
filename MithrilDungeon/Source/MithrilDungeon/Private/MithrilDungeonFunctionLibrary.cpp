// Fill out your copyright notice in the Description page of Project Settings.


#include "MithrilDungeonFunctionLibrary.h"
#include <Kismet/KismetRenderingLibrary.h>
#include <Kismet/GameplayStatics.h>
#include "MithrilDungeonGameInstance.h"
#include "ParentItem.h"
#include <Misc/Paths.h>

UTexture2D* UMithrilDungeonFunctionLibrary::GetImageByItemName(UObject* WorldContextObject, const FString& rowName)
{
	UTexture2D* rtn = nullptr;

	auto gameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);

	if (gameInstance != nullptr)
	{
		auto MithrilInstance = CastChecked<UMithrilDungeonGameInstance>(gameInstance);

		if (MithrilInstance != nullptr)
		{
			FParentItemInfo itemInfo = MithrilInstance->GetParentItemDataTable(rowName);

			UKismetSystemLibrary::PrintString(WorldContextObject, FString::Printf(TEXT("%s"), *(FPaths::ProjectDir() + itemInfo.itemImagePath)));

			rtn = UKismetRenderingLibrary::ImportFileAsTexture2D(WorldContextObject, FPaths::ProjectDir() + itemInfo.itemImagePath);
		}
	}

	return rtn;
}
