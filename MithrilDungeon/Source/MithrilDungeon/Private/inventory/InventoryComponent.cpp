// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryComponent.h"
#include "inventory/ItemBase.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

UItemBase* UInventoryComponent::FindMatchingItem(UItemBase* ItemIn) const
{
	if (ItemIn) // �����Ͱ� ��ȿ���� Ȯ��
	{
		if (InventoryContents.Contains(ItemIn))
		{
			return ItemIn;
		}
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindNextItemByID(UItemBase* ItemIn) const
{
	if (ItemIn)
	{
		if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result = InventoryContents.FindByKey(ItemIn)) // Ű�� �׸�ã��, �۵��ϸ� ��������� ��������.
		{
			return *Result;
		}
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindNextPartialStack(UItemBase* ItemIn) const
{
	if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result = InventoryContents.FindByPredicate([&ItemIn] // ����������
	(const UItemBase* InventoryItem) 
		{

			return InventoryItem->ID == ItemIn->ID && !InventoryItem->IsFullItemStack(); // ID�� ���� Ǯ��������������
		}
		))
	{
		return *Result;
	}
	return nullptr;
}

int32 UInventoryComponent::CalculateWeightAddAmount(UItemBase* ItemIn, int32 RequestedAddAmount)
{
	const int32 WeightMaxAddAmount = FMath::FloorToInt((GetWeightCapacity() - InventoryTotalWeight) / ItemIn->GetItemSingleWeight()); // �������߷����� ���߷����� �߷��뷮�� 50�̰� 20������ϸ� 30�̵�, �ε��Ҽ����� ������ ���尡��������� ������.
	if (WeightMaxAddAmount >= RequestedAddAmount) // �ִ��߷��� �����߷����� ũ�ų����ٸ� ��û�� ���ڹ�ȯ
	{
		return RequestedAddAmount;
	}
	return WeightMaxAddAmount;
}

int32 UInventoryComponent::CalculateNumberForFullStack(UItemBase* StackableItem, int32 InitialRequestedAddAmount)
{
	const int32 AddAmountToMakeFullStack = StackableItem->NumericData.MaxStackSize - StackableItem->Quantity; // �����͸� �������� �ִ뽺��ũ�⸦ �����ͼ� ��������� ��

	return FMath::Min(InitialRequestedAddAmount, AddAmountToMakeFullStack); // ���� �� ���� �� ��ȯ
}

void UInventoryComponent::RemoveSingleInstanceOfItem(UItemBase* ItemToRemove)
{
	InventoryContents.RemoveSingle(ItemToRemove);// �����׸� ����
	OnInventoryUpdated.Broadcast();

}

int32 UInventoryComponent::RemoveAmountOfItem(UItemBase* ItemIn, int32 DesiredAmountToRemove)
{
	const int32 ActualAmountToRemove = FMath::Min(DesiredAmountToRemove, ItemIn->Quantity); // �׸��� �� ����

	ItemIn->SetQuantity(ItemIn->Quantity - ActualAmountToRemove); // ��������, �׸��� ��������ǥ��, ������ �������� �� ���� ���ϴ� ���� �����ؼ� ��������

	InventoryTotalWeight -= ActualAmountToRemove * ItemIn->GetItemSingleWeight();
	
	OnInventoryUpdated.Broadcast();// ������Ʈ�ϰ� 
	 
	return ActualAmountToRemove; // ������ ������ �� ��ȯ
}

void UInventoryComponent::SplitExistingStack(UItemBase* ItemIn, const int32 AmountToSplit)
{
	if (!(InventoryContents.Num() + 1 > InventorySlotsCapacity)) // �κ��丮�� ��ġ�������� ����
	{
		RemoveAmountOfItem(ItemIn, AmountToSplit);
		AddNewItem(ItemIn, AmountToSplit);
	}
}

FItemAddResult UInventoryComponent::HandleNonStackableItems(UItemBase* InputItem, int32 RequestedAddAmount)
{
	// ��ȿ�� ����ġ�� �ִ��� Ȯ��, �뷮�̾������
	if (FMath::IsNearlyZero(InputItem->GetItemSingleWeight()) || InputItem->GetItemSingleWeight() < 0)
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString("Could not add {0} to the inventory. Item has invalid weight value."), InputItem->TextData.Name)); // �߰���������
	}
	
	// will the item weight overflow weight capacity �߰��ϸ� �뷮�ʰ�
	if (InventoryTotalWeight + InputItem->GetItemSingleWeight() > GetWeightCapacity())
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString("Could not add {0} to the inventory. Item would overflow weight limit."), InputItem->TextData.Name));
	}

	// adding one more item would overflow slot capacity �׸��� �ϳ� �� �߰��ϸ� ���Կ뷮�ʰ�
	if (InventoryContents.Num() + 1 > InventorySlotsCapacity)
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString("Could not add {0} to the inventory. All inventory slots are full."), InputItem->TextData.Name));
	}

	// �� ��������� ����
	AddNewItem(InputItem, RequestedAddAmount);
	return FItemAddResult::AddedAll(RequestedAddAmount,FText::Format(FText::FromString("Successfully added {0} {1} to the inventory."), RequestedAddAmount, InputItem->TextData.Name));
}

int32 UInventoryComponent::HandleStackableItems(UItemBase* ItemIn, int32 RequestedAddAmount)
{
	return int32();
}

FItemAddResult UInventoryComponent::HandleAddItem(UItemBase* InputItem)
{
	if (GetOwner())
	{
		const int32 InitialRequestedAddAmount = InputItem->Quantity;
		// handle non-stackable items
		if (!InputItem->NumericData.bisStackable)
		{
			return HandleNonStackableItems(InputItem, InitialRequestedAddAmount);
		}
	
		// handle stakckable �� �߰�
		const int32 StackableAmountAdded = HandleStackableItems(InputItem, InitialRequestedAddAmount);

		if (StackableAmountAdded == InitialRequestedAddAmount)
		{
			// return added all result
		}

		if (StackableAmountAdded < InitialRequestedAddAmount && StackableAmountAdded > 0)
		{
			// return added partial result
		}

		if (StackableAmountAdded <= 0) // �߰��� ���� 0���� ������ �ȵ�����
		{
			// return added none result
		}
	}
	return HandleNonStackableItems(InputItem, 3); // ���󺸰� ����������� �����ۺ��̽�������
}

void UInventoryComponent::AddNewItem(UItemBase* Item, const int32 AmountToAdd)
{
	UItemBase* NewItem;

	if (Item->bIsCopy || Item->bIsPickup)
	{
		// �׸��� �̹� �纻�̰ų� �Ⱦ��� ���
		NewItem = Item;
		NewItem->ResetItemFlags();
	}
	else
	{
		// �׷����������
		NewItem = Item->CreateItemCopy(); // �׸� ���纻�� �����ϰ� ȣ��
	}

	NewItem->OwningInventory = this; // �κ��丮�� �������� �������
	NewItem-> SetQuantity(AmountToAdd);	// �߰��� ��������

	InventoryContents.Add(NewItem);	// �� �׸� �߰�
	InventoryTotalWeight += NewItem->GetItemStackWeight();// ����ġ ����
	OnInventoryUpdated.Broadcast(); // ��� UI��ҿ� ������� ����
}