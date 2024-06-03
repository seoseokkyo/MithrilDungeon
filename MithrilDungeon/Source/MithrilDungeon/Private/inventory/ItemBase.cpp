// Fill out your copyright notice in the Description page of Project Settings.


#include "inventory/ItemBase.h"
#include <../../../../../../../Source/Runtime/Core/Public/Math/UnrealMathUtility.h>
#include "MithrilDungeonCharacter.h"

UItemBase::UItemBase()
{
	

}

UItemBase* UItemBase::CreateItemCopy() const
{
	UItemBase* ItemCopy = NewObject<UItemBase>(StaticClass()); // ���ο� ������ �������, ����Ŭ������ ����.

	ItemCopy-> ID =  this->ID; // �����ϴ� ������ �ǹ�.
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
		Quantity = FMath::Clamp(NewQuantity, 0 , NumericData.bisStackable ? NumericData.MaxStackSize : 1); // ��, ����

		/*if (OwningInventory)
		{
			if (Quantity <= 0)
			{
				owningInventory->RemoveItem(this); ���� 0 ������ �ǹ̾�� �� �׸��� ������.
			}
		}*/
	}

}

void UItemBase::Use(AMithrilDungeonCharacter* Character)
{
	

}