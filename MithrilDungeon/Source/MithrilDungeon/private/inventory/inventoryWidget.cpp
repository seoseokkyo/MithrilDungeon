// Fill out your copyright notice in the Description page of Project Settings.


#include "inventory/inventoryWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/UniformGridPanel.h>




void UinventoryWidget::inventoryOpen()
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			auto line = CreateWidget(this, inventoryBox);
			inventoryPanel->AddChildToUniformGrid(line, i, j);
		}
	}

}


