// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/InventoryPanel.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WrapBox.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include "MithrilDungeonCharacter.h"
#include "inventory/InventoryComponent.h"
#include "Inventory/InventoryItemSlot.h"

void UInventoryPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerCharacter = Cast<AMithrilDungeonCharacter>(GetOwningPlayerPawn());
	if (PlayerCharacter)
	{
		InventoryReference = PlayerCharacter->GetInventory(); // 인벤토리를 가져오고 인벤토리 가져오기를 기억
		if (InventoryReference)
		{
			InventoryReference->OnInventoryUpdated.AddUObject(this, &UInventoryPanel::RefreshInventory); // 인벤토리 패널전달 -> 인벤토리 새로고침
			SetInfoText();
		}
	}

}

// 무게 정보와 용량을 설정하기 위한것
void UInventoryPanel::SetInfoText() const
{
	// 무게
	WeightInfo->SetText(FText::Format(FText::FromString("{0}/{1}"),
	InventoryReference->GetInventoryTotalWeight(),
	InventoryReference->GetWeightCapacity()));

	// 용량
	CapacityInfo->SetText(FText::Format(FText::FromString("{0}/{1}"),
	InventoryReference->GetInventoryContents().Num(),
	InventoryReference->GetSlotsCapacity()));

}

void UInventoryPanel::RefreshInventory()
{
	if (InventoryReference && InventorySlotClass)
	{
		InventoryPanel->ClearChildren();

		for (UItemBase* const& InventoryItem : InventoryReference->GetInventoryContents())
		{
			UInventoryItemSlot* ItemSlot = CreateWidget<UInventoryItemSlot>(this, InventorySlotClass);
			ItemSlot->SetItemReference(InventoryItem);
			
			InventoryPanel->AddChildToWrapBox(ItemSlot);
		}

		SetInfoText();
	}

}





bool UInventoryPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return false;
}
