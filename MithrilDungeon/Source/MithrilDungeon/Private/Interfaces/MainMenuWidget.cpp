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

	PlayerCharacter = Cast<AMithrilDungeonCharacter>(GetOwningPlayerPawn()); // �⺻������ HUD�� �÷��̾� ��Ʈ�ѷ� �� �÷��̾�� ����Ǿ�����.

}

// ���콺�� Ŭ���ϰ� �巡���Ҷ� �����ͺ���
bool UMainMenuWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	// Cast Operation to item drag drop, ensure player is valid, call drop item on player �÷��̾ �÷��̾�� ��ȿ�� ȣ�� ����׸����� Ȯ��


	return false;
}
