// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/InteractionWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interfaces/InteractionInterface.h"

void UInteractionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InteractionProgressBar->PercentDelegate.BindUFunction(this, "UpdateInteractionProgress"); // �ۼ�Ʈ ������Ʈ

}

void UInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	KeyPressText->SetText(FText::FromString("FPress"));
	CurrentInteractionDuration = 0.0f; //���� ��ȣ�ۿ� �ð�
}

void UInteractionWidget::UpdateWidget(const FInteractableData* InteractableData) const
{
	switch (InteractableData->InteractableType)
	{
	case EInteractableType::Pickup:
	KeyPressText->SetText(FText::FromString("Press"));
	InteractionProgressBar->SetVisibility(ESlateVisibility::Collapsed);

	if (InteractableData->Quantity < 2) // ������ 1�̸� �Ⱥ��̵���
	{
		QuantityText->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		QuantityText->SetText(FText::Format(FText::FromString("x{0}"), InteractableData->Quantity)); // �μ� 0�������� ���⿡ �˰��ִ� �ٸ����ڰ� ������ �ű⿡����.
		QuantityText->SetVisibility(ESlateVisibility(Visibility));
	}
		break;

	case EInteractableType::NonPlayerCharacter:
		break;

	case EInteractableType::Device:
		break;

	case EInteractableType::Toggle:
		break;

	case EInteractableType::Container:
		break;

	default:
		break;
	}

	ActionText->SetText(InteractableData->Action);
	NameText->SetText(InteractableData->Name);
}

float UInteractionWidget::UpdateInteractionProgress()
{


	return 0.0f;
}


