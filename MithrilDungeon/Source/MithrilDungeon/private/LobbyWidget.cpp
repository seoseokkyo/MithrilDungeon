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

	// GI를 채우고싶다.
	GI = Cast<UMithrilDungeonGameInstance>(GetWorld()->GetGameInstance());

	// GI의 OnMySessionSearchCompleteDelegate에 AddRoomInfoUI를 연결하고싶다.
	GI->OnMySessionSearchCompleteDelegate.AddDynamic(this, &ULobbyWidget::AddRoomInfoUI);

	// 버튼을 연결하고싶다.
	Button_CreateRoom->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyclickCreateRoom);

	Slider_PlayerCount->SetValue(FCString::Atof(*Text_PlayerCount->GetText().ToString()));

	Slider_PlayerCount->OnValueChanged.AddDynamic(this, &ULobbyWidget::OnMyValueChanged);

	Button_GoCreateRoom->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickGoCreateRoom);
	Button_GoFindRoom->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickGoFindRoom);
	Button_Menu_1->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickGoMenu);
	Button_Menu_2->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickGoMenu);

	Button_FindRoom->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickFindRoom);

	// 최초 시작시 메뉴위젯을 활성화 하고싶다.
	SwitcherUI->SetActiveWidgetIndex(0);

}

void ULobbyWidget::OnMyClickGoMenu()
{
	SwitcherUI->SetActiveWidgetIndex(0); // 이넘으로 만들어서 이름으로 바꿀수도 있음.
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

void ULobbyWidget::OnMyValueChanged(float value) // 슬라이더 움직이면 호출됨
{
	Text_PlayerCount->SetText(FText::AsNumber(value));
}

void ULobbyWidget::OnMyClickFindRoom()
{
	// GI의 FindOtherSessions를 호출하고싶다.
	GI->FindOtherSessions();

}

void ULobbyWidget::AddRoomInfoUI(const FSessionInfo& info)
{
	// RoomInfoUIFactory를 이용해서 위젯을 만들고
	auto ui = CreateWidget<URoomInfoUI>(this, RoomInfoUIFactory);
	// info를 Setup함수를 통해 전달하고싶다.
	ui->Setup(info);
	// 생성한 위젯을 ScrollBox_RoomList에 붙이고싶다.
	ScrollBox_RoomList->AddChild(ui);
}