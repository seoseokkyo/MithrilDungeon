// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "MithrilDungeonCharacter.h"
#include "itemDragDropOperation.generated.h"

// Operation
/**
 * 
 */
UCLASS()
class MITHRILDUNGEON_API UitemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:

	
	// ȭ�鿡�� �巡���Ϸ��� UMG���� ��µ� ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* WidgetReference;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D DragOffset;


	
};
