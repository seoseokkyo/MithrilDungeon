// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class MITHRILDUNGEON_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY()
	class UMithrilDungeonGameInstance* GI;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UWidgetSwitcher* SwitcherUI; // 0 : �޴�, 1 : �����, 2 : ��ã��

	UFUNCTION()
	void OnMyClickGoMenu();

	// Menu
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* Button_GoCreateRoom;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* Button_GoFindRoom;

	UFUNCTION()
	void OnMyClickGoCreateRoom();

	UFUNCTION()
	void OnMyClickGoFindRoom();

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UEditableText* Edit_RoomName;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class USlider* Slider_PlayerCount;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* Text_PlayerCount;

	// Button_CreateRoom�� ������ GameInstance�� CreateMySession �Լ��� ȣ���ϰ�ʹ�.

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* Button_CreateRoom;

	UFUNCTION()
	void OnMyclickCreateRoom();

	UFUNCTION()
	void OnMyValueChanged(float value);

	
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* Button_Menu_1;

	// Find Room

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class URoomInfoUI> RoomInfoUIFactory;


	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UScrollBox* ScrollBox_RoomList;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* Button_FindRoom;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* Button_Menu_2;

	UFUNCTION()
	void OnMyClickFindRoom();

	UFUNCTION()
	void AddRoomInfoUI(const struct FSessionInfo& info);

};
