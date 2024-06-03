// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/MainMenuWidget.h"
#include "MithrilDungeonCharacter.h"

void UMainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter = Cast<AMithrilDungeonCharacter>(GetOwningPlayerPawn()); // 기본적으로 HUD는 플레이어 컨트롤러 및 플레이어와 연결되어있음.

}

// 마우스를 클릭하고 드래그할때 데이터보관
bool UMainMenuWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	// Cast Operation to item drag drop, ensure player is valid, call drop item on player 플레이어가 플레이어에서 휴효한 호출 드롭항목인지 확인


	return false;
}
