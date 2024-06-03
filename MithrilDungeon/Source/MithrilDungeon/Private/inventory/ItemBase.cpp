// Fill out your copyright notice in the Description page of Project Settings.


#include "inventory/ItemBase.h"
#include <../../../../../../../Source/Runtime/Core/Public/Math/UnrealMathUtility.h>
#include "MithrilDungeonCharacter.h"

UItemBase::UItemBase()
{
	

}

UItemBase* UItemBase::CreateItemCopy() const
{
	UItemBase* ItemCopy = NewObject<UItemBase>(StaticClass()); // 새로운 아이템 생성기능, 정적클래스를 얻음.

	ItemCopy-> ID =  this->ID; // 복사하는 원본을 의미.
	ItemCopy->Quantity = this->Quantity;
	ItemCopy->ItemQuality = this->ItemQuality;
	ItemCopy->ItemType = this->ItemType;
	ItemCopy->TextData = this->TextData;
	ItemCopy->NumericData = this->NumericData;
	ItemCopy->ItemStatistics = this ->ItemStatistics;
	ItemCopy->AssetData = this->AssetData;


	return ItemCopy;
}

void UItemBase::SetQuantity(const int32 NewQuantity)
{
	if (NewQuantity != Quantity)
	{
		Quantity = FMath::Clamp(NewQuantity, 0 , NumericData.bisStackable ? NumericData.MaxStackSize : 1); // 참, 거짓

		/*if (OwningInventory)
		{
			if (Quantity <= 0)
			{
				owningInventory->RemoveItem(this); 수량 0 유지는 의미없어서 그 항목은 제거함.
			}
		}*/
	}

}

void UItemBase::Use(AMithrilDungeonCharacter* Character)
{
	

}
