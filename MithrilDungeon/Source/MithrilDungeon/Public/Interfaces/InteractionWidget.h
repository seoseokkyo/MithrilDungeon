// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

class AMithrilDungeonCharacter;
struct FInteractableData; // �����Ϸ��� ���� �ٸ������� ���ǵ� �������� �� �� �ֵ��� �� ���� Ŭ���� �ܺ� ��򰡿� ��ġ�ϴ� �Ͱ� ������ ������� ��κ��� �׸�����
class UTextBlock;
class UProgressBar;

/**
 * 
 */
UCLASS()
class MITHRILDUNGEON_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "Interaction Widget | Player Reference")
	AMithrilDungeonCharacter* PlayerReference;

	void UpdateWidget(const FInteractableData* InteractableData)const; // ��� �ٸ� �ؽ�Ʈ��� ������Ʈ

protected:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UTextBlock* NameText; // �̸�

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UTextBlock* ActionText; // �۾�

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UTextBlock* QuantityText; // ����

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UTextBlock* KeyPressText; // Ű������ �ؽ�Ʈ

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UProgressBar* InteractionProgressBar; // �ð�ǥ����

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	float CurrentInteractionDuration;

	UFUNCTION(Category = "Interaction Widget | Interactable Data")
	float UpdateInteractionProgress();// �Ҽ��� ��ȯ�Լ�, ��ȣ�ۿ� ����� ������Ʈ

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

};
