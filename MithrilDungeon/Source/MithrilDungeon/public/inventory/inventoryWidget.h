// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "inventoryWidget.generated.h"
// MyWindow

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
	
	// �κ��丮 ����10�� ���� 6��
	UPROPERTY(EditDefaultsOnly, Category = TPS)
	TSubclassOf<class UUserWidget> inventoryBox;

	void inventoryOpen();

	// �κ��丮 ��Ӿ� ���
	UPROPERTY(EditAnywhere, Category = "Drop|ItemWidget")
	TSubclassOf<UUserWidget> DropWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Drop|ItemWidget")
	class UDropItemWidget* DropitemWidget;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class USizeBox* sizeBox;

	

private:

	// �κ��丮 ��Ӿ� ���
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* Inpoeration) override;

	/*FReply CustomDetectDrag(const FPointerEvent& InMouseEvent, UWidget* WIdgetDetectingDrag, FKey DragKey);*/

	virtual void NativeOnInitialized() override;
};
