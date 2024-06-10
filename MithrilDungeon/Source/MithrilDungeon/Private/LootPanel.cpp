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

	PlayerCharacter = Cast<ADungeonOrganism>(GetOwningPlayerPawn()); // 기본적으로 HUD는 플레이어 컨트롤러 및 플레이어와 연결되어있음.
}

// 마우스를 클릭하고 드래그할때 데이터보관
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

	// Cast Operation to item drag drop, ensure player is valid, call drop item on player 플레이어가 플레이어에서 휴효한 호출 드롭항목인지 확인

}

