// Fill out your copyright notice in the Description page of Project Settings.


#include "MithrilDungeonGameInstance.h"
#include "StateComponent.h"

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