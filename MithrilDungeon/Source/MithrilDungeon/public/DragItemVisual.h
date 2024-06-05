// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DragItemVisual.generated.h"

class UBorder;
class UTextBlock;
class UImage;

/**
 * 
 */
UCLASS()
class MITHRILDUNGEON_API UDragItemVisual : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere,Category = "Drag Item Visual", meta = (BindWidget))
	UBorder* ItemBorder;

	UPROPERTY(VisibleAnywhere,Category = "Drag Item Visual", meta = (BindWidget))
	UImage* ItemIcon;

	UPROPERTY(VisibleAnywhere,Category = "Drag Item Visual", meta = (BindWidget))
	UTextBlock* ItemQuantity;


};
