// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Pickup.generated.h"

class UDataTable;
class UItemBase;
class ADungeonOrganism;

UCLASS()
class MITHRILDUNGEON_API APickup : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	//================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================
	

	//================================================================================
	// FUNCTIONS
	//================================================================================
	APickup();

	void InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity); // �Ⱦ� �ʱ�ȭ

	void InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity); // ��� �ʱ�ȭ

	FORCEINLINE UItemBase* GetItemData() { return ItemReference; };

	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	

protected:
	//================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================
	UPROPERTY(VisibleAnywhere, Category = "Pickup | Components")
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	UDataTable* ItemDataTable;


	UPROPERTY(VisibleAnywhere, Category = "Pickup | Item Reference")
	UItemBase* ItemReference; // �Ⱦ��� ����Ű�� �׸�

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	int32 ItemQuantity; // ����

	UPROPERTY(VisibleInstanceOnly, Category = "Pickup | Interaction")
	FInteractableData InstanceInteractableData; // �ν��Ͻ� ��ȣ�ۿ밡��


	//================================================================================
	// FUNCTIONS
	//================================================================================
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(AMithrilDungeonCharacter* PlayerCharacter) override;
	void UpdateInteractableData();

	void TakePickup(const ADungeonOrganism* Taker); // ĳ������ �κ��丮�� �������̽��ؾ���

#if WITH_EDITOR // ��ũ��
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	FName DesiredItemID;

	void SetInput(const ADungeonOrganism* Taker);
};
