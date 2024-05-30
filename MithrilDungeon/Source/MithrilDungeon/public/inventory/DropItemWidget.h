// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DropItemWidget.generated.h"

// HUD ����

/**
 * 
 */
UCLASS()
class MITHRILDUNGEON_API UDropItemWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;



};
