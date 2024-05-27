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

	// 플레이어가 E키를 누르면 인벤토리가 열리도록
	
	// 인벤토리 가로10줄 세로 6줄
	UPROPERTY(EditDefaultsOnly, Category = TPS)
	TSubclassOf<class UUserWidget> inventoryBox;


	void inventoryOpen();



};
