// Fill out your copyright notice in the Description page of Project Settings.


#include "MithrilDungeonGameInstance.h"
#include "StateComponent.h"
#include "ParentItem.h"

FCharacterStat UMithrilDungeonGameInstance::GetCharacterDataTable(const FString& rowName)
{
	if (dt_characerStatDataTable != nullptr)
	{
		FString errorText;
		FCharacterStat* characerStatInfo = dt_characerStatDataTable->FindRow<FCharacterStat>(FName(rowName), errorText);

		if (characerStatInfo != nullptr)
		{
			return *characerStatInfo;
		}
		else
		{
			return FCharacterStat();
		}
	}

	return FCharacterStat();
}

FParentItemInfo UMithrilDungeonGameInstance::GetParentItemDataTable(const FString& rowName)
{
	if (dt_parentItemDataTable != nullptr)
	{
		FString errorText;
		FParentItemInfo* parentItemInfo = dt_parentItemDataTable->FindRow<FParentItemInfo>(FName(rowName), errorText);

		if (parentItemInfo != nullptr)
		{
			return *parentItemInfo;
		}
		else
		{
			return FParentItemInfo();
		}
	}

	return FParentItemInfo();
}
