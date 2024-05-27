// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "inventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class MITHRILDUNGEON_API UinventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UUniformGridPanel* inventoryPanel;

	// �÷��̾ EŰ�� ������ �κ��丮�� ��������
	
	// �κ��丮 ����10�� ���� 6��
	UPROPERTY(EditDefaultsOnly, Category = TPS)
	TSubclassOf<class UUserWidget> inventoryBox;


	void inventoryOpen();



};
