// Fill out your copyright notice in the Description page of Project Settings.


#include "portal/portalBarWidget.h"
#include "Components/ProgressBar.h"





void UportalBarWidget::NativeConstruct()
{
	Super::NativeConstruct();



	Portal_ProgressBar->SetPercent(0.0f);



}

void UportalBarWidget::portalBarUpdate(float Update)
{
	Portal_ProgressBar->SetPercent(Update / 8.0f);
}


