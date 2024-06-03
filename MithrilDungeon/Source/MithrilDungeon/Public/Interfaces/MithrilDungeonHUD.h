// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MithrilDungeonHUD.generated.h"

class UMainMenuWidget; // ���� ���ƴ��� �˾ƺ�������, ������� �߰� ���ص� ��.
class UInteractionWidget;
struct FInteractableData;
// HUD�� ������ �����ϱ� �Ϻ��� ���
/**
 * 
 */
UCLASS()
class MITHRILDUNGEON_API AMithrilDungeonHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	//================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================
	 
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;

	bool bIsMenuVisible; // ���ظ�忡 �ִ� �����޴� ��ȯX�ϱ����� �Ұ�

	//================================================================================
	// FUNCTIONS
	
	AMithrilDungeonHUD();


	void DisplayMenu(); // ǥ�ø޴����
	void HideMenu(); // �޴������

	void ShowInteractionWidget()const;
	void HideInteractionWidget()const;
	void UpdateInteractionWidget(const FInteractableData* InteractableData)const; // ��ȣ�ۿ밡�ɵ�����

	//================================================================================
	

protected:

	//================================================================================
	// PROPERTIES & VARIABLES
	UPROPERTY()
	UMainMenuWidget* MainManuWidget;

	UPROPERTY()
	UInteractionWidget* InteractionWidget;
	 
	//================================================================================

	//================================================================================
	// FUNCTIONS
	//================================================================================
	virtual void BeginPlay() override;

};
