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

	InitializePickup(UItemBase::StaticClass(), ItemQuantity); // 수량설정
	

}


void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickup::InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity)
{
	if (ItemDataTable && !DesiredItemID.IsNone()) // 없는 아이템인지 확인 ,비어있지않음
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());

		ItemReference = NewObject<UItemBase>(this, BaseClass); // 항목을 성공적으로 가져오고 초기화 할 새 항목참조생성

		// 새 항목 데이터 테이블 행의 항목으로 내부 픽업 항목 참조를 초기화
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
	ItemReference->NumericData.Weight = ItemToDrop->GetItemSingleWeight(); // 항목드롭설정
	PickupMesh->SetStaticMesh(ItemToDrop->AssetData.Mesh); // 드롭할 항목데이터 구조메시확인

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
		TakePickup(PlayerCharacter); // 픽업요청, 플레이어캐릭터 전달
	}
}



void APickup::TakePickup(const AMithrilDungeonCharacter* Taker)
{
	if (!IsPendingKillPending()) // 항목추가처리
	{
		if (ItemReference)
		{
			//if (UInventoryComponent* PlayerInventory = Taker->GetInventory());

			// 플레이어 인벤토리에 항목을 추가한 다음
			// 결과에따라 항목을 조정하거나 파괴.
		}
	}
}

void APickup::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	// 이름이 유효하면 이름반환, 유효하지않으면 이름없음을 반환

	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(APickup, DesiredItemID)) // 사물의 유효성을 확인하는 매크로
	{
		if (ItemDataTable)
		{
			if (const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString())) // 유효한 행을 얻으면 즉시 최적화
			{
				PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh); // 픽업메시를 데이터메시로 설정
			}
		}
	}

}

