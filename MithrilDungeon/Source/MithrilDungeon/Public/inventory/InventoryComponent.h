// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdated); // ��Ƽĳ��Ʈ �븮�ڼ���

class UItemBase;

UENUM(BlueprintType)
enum class EItemAddResult : uint8
{
	IAR_NoItemAdded UMETA(DisplayName = "No item added"), // �������� �������
	IAR_PartialAmountItemAdded UMETA(DisplayName = "Partial amount of item added"),
	IAR_AllItemAdded UMETA(DisplayName = "All of item added"), // �������������
};

USTRUCT(BlueprintType)
struct FItemAddResult
{
	GENERATED_BODY()

	FItemAddResult() : 
	ActualAmountAdded(0),
	operationResult(EItemAddResult::IAR_NoItemAdded),
	ResultMessage(FText::GetEmpty())
	{}; // �Լ� �����ʿ� x

	// Actual amount of item that was added to the Inventory �κ��丮�� �߰��� �׸��� ���� ��
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	int32 ActualAmountAdded; // �츮�� ���ϴ� �ʵ忡 ���� �ݾ��� �߰�

	// Enum representing the end state of an add item operation  �׸� �߰� �۾��� ���� ���¸� ��Ÿ���� ����
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	EItemAddResult operationResult;

	// Informational message that can be passed with the result ����� �Բ� ������ �� �ִ� ���� �޽���
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	FText ResultMessage;

	static FItemAddResult AddedNone(const FText& ErrorText)// ���ο� �׸��߰� ����� ������ �ʿ䰡 ���ٴ°��� �ǹ�
	{
		FItemAddResult AddedNoneResult;
		AddedNoneResult.ActualAmountAdded = 0;
		AddedNoneResult.operationResult = EItemAddResult::IAR_NoItemAdded;
		AddedNoneResult.ResultMessage = ErrorText;
		return AddedNoneResult;
	}; 

	static FItemAddResult AddedPartial(const int32 PartialAmountAdded, const FText& ErrorText)
	{
		FItemAddResult AddedPartialResult;
		AddedPartialResult.ActualAmountAdded = PartialAmountAdded;
		AddedPartialResult.operationResult = EItemAddResult::IAR_PartialAmountItemAdded;
		AddedPartialResult.ResultMessage = ErrorText;
		return AddedPartialResult;
	};
	
	static FItemAddResult AddedAll(const int32 AmountAdded, const FText& Message)
	{
		FItemAddResult AddedAllResult;
		AddedAllResult.ActualAmountAdded = AmountAdded;
		AddedAllResult.operationResult = EItemAddResult::IAR_AllItemAdded;
		AddedAllResult.ResultMessage = Message;
		return AddedAllResult;
	};

	
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MITHRILDUNGEON_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	//================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================
	FOnInventoryUpdated OnInventoryUpdated; // �븮���� �ν��Ͻ� �����


	//================================================================================
	// FUNCTIONS
	//================================================================================
	UInventoryComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(Category = "Inventory")
	FItemAddResult HandleAddItem(UItemBase* InputItem);

	// ã����
	UFUNCTION(Category = "Inventory")
	UItemBase* FindMatchingItem(UItemBase* ItemIn) const;

	UFUNCTION(Category = "Inventory")
	UItemBase* FindNextItemByID(UItemBase* ItemIn) const;
	
	UFUNCTION(Category = "Inventory")
	UItemBase* FindNextPartialStack(UItemBase* ItemIn) const; // �κн���ã��

	// ���ű��
	UFUNCTION(Category = "Inventory")
	void RemoveSingleInstanceOfItem(UItemBase* ItemIn);
	
	UFUNCTION(Category = "Inventory")
	int32 RemoveAmountOfItem(UItemBase* ItemToRemove, int32 DesiredAmountToRemove);
	
	UFUNCTION(Category = "Inventory")
	void SplitExistingStack(UItemBase* ItemIn, const int32 AmountToSplit);

	// ���ұ��, getters
	// -----------------------------------------
	UFUNCTION(Category = "Inventory")
	FORCEINLINE float GetInventoryTotalWeight() const { return InventoryTotalWeight; };
	
	UFUNCTION(Category = "Inventory")
	FORCEINLINE float GetWeightCapacity() const { return InventoryWeightCapacity; }; // ����
	
	UFUNCTION(Category = "Inventory")
	FORCEINLINE int32 GetSlotsCapacity() const { return InventorySlotsCapacity; }; // ����
	
	UFUNCTION(Category = "Inventory")
	FORCEINLINE TArray<UItemBase*> GetInventoryContents() const { return InventoryContents; }; // �κ� �迭

	//setters
	// -----------------------------------------
	UFUNCTION(Category = "Inventory")
	FORCEINLINE void SetSlotsCapacity(const int32 NewSlotsCapacity) { InventorySlotsCapacity = NewSlotsCapacity; }; // ���Կ뷮 ����
	
	UFUNCTION(Category = "Inventory")
	FORCEINLINE void SetWeightCapacity(const float NewWeightCapacity) { InventoryWeightCapacity = NewWeightCapacity; };

protected:
	

	//================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	float InventoryTotalWeight; 
	
	UPROPERTY(EditInstanceOnly, Category = "Inventory")
	int32 InventorySlotsCapacity;
	
	UPROPERTY(EditInstanceOnly, Category = "Inventory")
	float InventoryWeightCapacity;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<TObjectPtr<UItemBase>> InventoryContents; // ���� ���� �����͸� t��ü �����ͷ� ��ü

	//================================================================================
	// FUNCTIONS
	//================================================================================

	virtual void BeginPlay() override;

	FItemAddResult HandleNonStackableItems(UItemBase* ItemIn);
	int32 HandleStackableItems(UItemBase* InputItem, int32 RequestedAddAmount);// ������ �μ��� ������ ���� �׸�ó��
	int32 CalculateWeightAddAmount(UItemBase* ItemIn, int32 RequestedAddAmount);// �����߰� ���
	int32 CalculateNumberForFullStack(UItemBase* StackableItem, int32 InitialRequestedAddAmount); // �ʱ��û�ݾ�

	void AddNewItem(UItemBase* Item, const int32 AmountToAdd);// ���׸��߰�
	
};
