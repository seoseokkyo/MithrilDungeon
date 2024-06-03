// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Pickup.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include "inventory/ItemBase.h"

// Sets default values
APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = false;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetSimulatePhysics(true);
	SetRootComponent(PickupMesh);

}


void APickup::BeginPlay()
{
	Super::BeginPlay();

	InitializePickup(UItemBase::StaticClass(), ItemQuantity); // ��������
	

}


void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickup::InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity)
{
	if (ItemDataTable && !DesiredItemID.IsNone()) // ���� ���������� Ȯ�� ,�����������
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());

		ItemReference = NewObject<UItemBase>(this, BaseClass); // �׸��� ���������� �������� �ʱ�ȭ �� �� �׸���������

		// �� �׸� ������ ���̺� ���� �׸����� ���� �Ⱦ� �׸� ������ �ʱ�ȭ
		ItemReference->ID = ItemData->ID;
		ItemReference->ItemType = ItemData->ItemType;
		ItemReference->ItemQuality = ItemData->ItemQuality;
		ItemReference->NumericData = ItemData->NumericData;
		ItemReference->TextData = ItemData->TextData;
		ItemReference->AssetData = ItemData->AssetData;

		InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);

		PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);

		UpdateInteractableData();
	}
}

void APickup::InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity)
{
	ItemReference = ItemToDrop;
	InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);
	ItemReference->NumericData.Weight = ItemToDrop->GetItemSingleWeight(); // �׸��Ӽ���
	PickupMesh->SetStaticMesh(ItemToDrop->AssetData.Mesh); // ����� �׸����� �����޽�Ȯ��

	UpdateInteractableData();
}

void APickup::UpdateInteractableData()
{
	InstanceInteractableData.InteractableType = EInteractableType::Pickup;
	InstanceInteractableData.Action = ItemReference->TextData.InteractionText;
	InstanceInteractableData.Name = ItemReference->TextData.Name;
	InstanceInteractableData.Quantity = ItemReference->Quantity;
	InteractableData = InstanceInteractableData;


}

void APickup::BeginFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(true);
	}

}

void APickup::EndFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(true);
	}
}


void APickup::Interact(AMithrilDungeonCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		TakePickup(PlayerCharacter); // �Ⱦ���û, �÷��̾�ĳ���� ����
	}
}



void APickup::TakePickup(const AMithrilDungeonCharacter* Taker)
{
	if (!IsPendingKillPending()) // �׸��߰�ó��
	{
		if (ItemReference)
		{
			//if (UInventoryComponent* PlayerInventory = Taker->GetInventory());

			// �÷��̾� �κ��丮�� �׸��� �߰��� ����
			// ��������� �׸��� �����ϰų� �ı�.
		}
	}
}

void APickup::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	// �̸��� ��ȿ�ϸ� �̸���ȯ, ��ȿ���������� �̸������� ��ȯ

	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(APickup, DesiredItemID)) // �繰�� ��ȿ���� Ȯ���ϴ� ��ũ��
	{
		if (ItemDataTable)
		{
			if (const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString())) // ��ȿ�� ���� ������ ��� ����ȭ
			{
				PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh); // �Ⱦ��޽ø� �����͸޽÷� ����
			}
		}
	}

}

