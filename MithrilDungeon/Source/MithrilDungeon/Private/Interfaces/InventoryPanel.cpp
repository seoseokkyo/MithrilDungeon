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
		InventoryReference = PlayerCharacter->GetInventory(); // �κ��丮�� �������� �κ��丮 �������⸦ ���
		if (InventoryReference)
		{
			InventoryReference->OnInventoryUpdated.AddUObject(this, &UInventoryPanel::RefreshInventory); // �κ��丮 �г����� -> �κ��丮 ���ΰ�ħ
			SetInfoText();
		}
	}

}

// ���� ������ �뷮�� �����ϱ� ���Ѱ�
void UInventoryPanel::SetInfoText() const
{
	// ����
	WeightInfo->SetText(FText::Format(FText::FromString("{0}/{1}"),
	InventoryReference->GetInventoryTotalWeight(),
	InventoryReference->GetWeightCapacity()));

	// �뷮
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
