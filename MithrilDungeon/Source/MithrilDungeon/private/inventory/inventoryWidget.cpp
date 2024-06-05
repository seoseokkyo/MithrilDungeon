// Fill out your copyright notice in the Description page of Project Settings.


#include "inventory/inventoryWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/UniformGridPanel.h>
#include <Components/SizeBox.h>
#include "inventory/DropItemWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h>
#include "inventory/itemDragDropOperation.h"



void UinventoryWidget::inventoryOpen()
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			auto line = CreateWidget(this, inventoryBox);
			inventoryPanel->AddChildToUniformGrid(line, i, j);
		}
		if (IsValid(DropWidgetClass))
		{
			DropitemWidget = Cast<UDropItemWidget>(CreateWidget(GetWorld(), DropWidgetClass));
			sizeBox->AddChild(DropitemWidget);
		}
	}
}

FReply UinventoryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	 Super::NativeOnMouseButtonDown(InGeometry,InMouseEvent);

	FEventReply reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);

	return reply.NativeReply;
}

void UinventoryWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UitemDragDropOperation* DragDropOperation = NewObject<UitemDragDropOperation>();
	this->SetVisibility(ESlateVisibility::HitTestInvisible);

	//DragDropOperation->WidgetReference = this;
	//DragDropOperation->DragOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

	DragDropOperation->DefaultDragVisual = this;
	DragDropOperation->Pivot = EDragPivot::MouseDown;

	OutOperation = DragDropOperation;
}

void UinventoryWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* Inpoeration)
{
	RemoveFromParent();
}

//FReply UinventoryWidget::CustomDetectDrag(const FPointerEvent& InMouseEvent, UWidget* WIdgetDetectingDrag, FKey DragKey)
//{
//
//
//	return FReply();
//}

void UinventoryWidget::NativeOnInitialized()
{

}


