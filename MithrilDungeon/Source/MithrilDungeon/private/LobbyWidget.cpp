// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"
#include <Components/Button.h>
#include <Components/EditableText.h>
#include <Components/Slider.h>
#include <Components/TextBlock.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WidgetSwitcher.h>
#include "RoomInfoUI.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/ScrollBox.h>
#include "MithrilDungeonGameInstance.h"


void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// GI�� ä���ʹ�.
	GI = Cast<UMithrilDungeonGameInstance>(GetWorld()->GetGameInstance());

	// GI�� OnMySessionSearchCompleteDelegate�� AddRoomInfoUI�� �����ϰ�ʹ�.
	GI->OnMySessionSearchCompleteDelegate.AddDynamic(this, &ULobbyWidget::AddRoomInfoUI);

	// ��ư�� �����ϰ�ʹ�.
	Button_CreateRoom->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyclickCreateRoom);

	Slider_PlayerCount->SetValue(FCString::Atof(*Text_PlayerCount->GetText().ToString()));

	Slider_PlayerCount->OnValueChanged.AddDynamic(this, &ULobbyWidget::OnMyValueChanged);

	Button_GoCreateRoom->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickGoCreateRoom);
	Button_GoFindRoom->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickGoFindRoom);
	Button_Menu_1->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickGoMenu);
	Button_Menu_2->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickGoMenu);

	Button_FindRoom->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickFindRoom);

	// ���� ���۽� �޴������� Ȱ��ȭ �ϰ�ʹ�.
	SwitcherUI->SetActiveWidgetIndex(0);

}

void ULobbyWidget::OnMyClickGoMenu()
{
	SwitcherUI->SetActiveWidgetIndex(0); // �̳����� ���� �̸����� �ٲܼ��� ����.
}

void ULobbyWidget::OnMyClickGoCreateRoom()
{
	SwitcherUI->SetActiveWidgetIndex(1);
}

void ULobbyWidget::OnMyClickGoFindRoom()
{
	SwitcherUI->SetActiveWidgetIndex(2);
}

void ULobbyWidget::OnMyclickCreateRoom()
{
	if (GI)
	{
		int32 count = Slider_PlayerCount->GetValue();
		GI->CreateMySession(Edit_RoomName->GetText().ToString(), count);
	}

}

void ULobbyWidget::OnMyValueChanged(float value) // �����̴� �����̸� ȣ���
{
	Text_PlayerCount->SetText(FText::AsNumber(value));
}

void ULobbyWidget::OnMyClickFindRoom()
{
	// GI�� FindOtherSessions�� ȣ���ϰ�ʹ�.
	GI->FindOtherSessions();

}

void ULobbyWidget::AddRoomInfoUI(const FSessionInfo& info)
{
	// RoomInfoUIFactory�� �̿��ؼ� ������ �����
	auto ui = CreateWidget<URoomInfoUI>(this, RoomInfoUIFactory);
	// info�� Setup�Լ��� ���� �����ϰ�ʹ�.
	ui->Setup(info);
	// ������ ������ ScrollBox_RoomList�� ���̰�ʹ�.
	ScrollBox_RoomList->AddChild(ui);
}