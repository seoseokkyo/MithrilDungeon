// Fill out your copyright notice in the Description page of Project Settings.


#include "LootPanel.h"
#include "MithrilDungeonCharacter.h"
#include "inventory/itemDragDropOperation.h"
#include "inventory/ItemBase.h"
#include "DungeonOrganism.h"

void ULootPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

}

void ULootPanel::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter = Cast<ADungeonOrganism>(GetOwningPlayerPawn()); // �⺻������ HUD�� �÷��̾� ��Ʈ�ѷ� �� �÷��̾�� ����Ǿ�����.
}

// ���콺�� Ŭ���ϰ� �巡���Ҷ� �����ͺ���
bool ULootPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	const UitemDragDropOperation* ItemDragDrop = Cast <UitemDragDropOperation>(InOperation);

	if (PlayerCharacter && ItemDragDrop->SourceItem)
	{
		auto playerCheck = Cast<AMithrilDungeonCharacter>(PlayerCharacter);
		playerCheck->DropItem(ItemDragDrop->SourceItem, ItemDragDrop->SourceItem->Quantity);
		return true;
	}
	return false;

	// Cast Operation to item drag drop, ensure player is valid, call drop item on player �÷��̾ �÷��̾�� ��ȿ�� ȣ�� ����׸����� Ȯ��

}

