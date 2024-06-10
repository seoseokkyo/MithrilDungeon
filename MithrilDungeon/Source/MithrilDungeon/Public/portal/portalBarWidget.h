// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "portalBarWidget.generated.h"


class UProgressBar;

/**
 * 
 */
UCLASS()
class MITHRILDUNGEON_API UportalBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;

	
	UPROPERTY(VisibleAnywhere, Category = "MySettings", meta = (BindWidget))
	UProgressBar* Portal_ProgressBar;
	
	void portalBarUpdate(float Update);




};
