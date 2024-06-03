// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

class AMithrilDungeonCharacter; // Ŭ�������漱��, �ٸ������� ���� �� ������ �˸�

UENUM()
enum class EInteractableType : uint8
{
	Pickup UMETA(DisPlayName = "PickUp"),
	NonPlayerCharacter UMETA(DisPlayName = "NonPlayerCharacter"),
	Device UMETA(DisPlayName = "Device"),
	Toggle UMETA(DisPlayName = "Toggle"),
	Container UMETA(DisPlayName = "Container")
};


USTRUCT()
struct FInteractableData
{
	GENERATED_USTRUCT_BODY()

	FInteractableData() :
	InteractableType(EInteractableType::Pickup),
	Name(FText::GetEmpty()), // FText ���� �ٽù�ȯ
	Action(FText::GetEmpty()),
	Quantity(0), // �����۰���
	InteractionDuration(0.0f) // ��ȣ�ۿ� �ð�
	{

	}; // �����ڰ� ����� �� ���⿡�� ù ��° �۾��� ����

	// ��ȣ�ۿ� ������ ������ ��ü�� ���纻�� ������.
	UPROPERTY(EditInstanceOnly)
	EInteractableType InteractableType; // ��ȣ�ۿ� ��������
	
	UPROPERTY(EditInstanceOnly)
	FText Name;
	
	UPROPERTY(EditInstanceOnly)
	FText Action;
	
	// used Only for Pickups
	UPROPERTY(EditInstanceOnly)
	int8 Quantity;

	// used for things like valves, doors, etc, that require an interaction timer
	UPROPERTY(EditInstanceOnly)
	float InteractionDuration; // ��ȣ�ۿ� ���ɽð�
};


// InteractionInterface ��ȣ�ۿ� �������̽�
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface // �ǹ� UE�������̽��� ����
{
	GENERATED_BODY()
};

/**
 * 
 */
class MITHRILDUNGEON_API IInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void BeginFocus(); // �Լ��� ���� Ŭ������ ���� ������ �� �� ������ ����
	virtual void EndFocus(); 
	virtual void BeginInteract(); // e�� ��� ���� ���� ���ۻ�ȣ�ۿ��� ����Ͽ� 1~2�� ���� ��ٸ���
	virtual void EndInteract();
	virtual void Interact(AMithrilDungeonCharacter* PlayerCharacter); // �÷��̾� ĳ���͸� ���� ��ȣ�ۿ��μ� ����

	FInteractableData InteractableData;//  ��ȣ�ۿ� ���� �����Ͷ�� ����ü
};
