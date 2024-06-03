// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

UENUM(BlueprintType)
enum class EItemAddResult : uint8
{
	IAR_NoItemAdded UMETA(DisplayName = "No item added"), // 아이템이 없을경우
	IAR_PartialAmountItemAdded UMETA(DisplayName = "Partial amount of item added"),
	IAR_AllItemAdded UMETA(DisplayName = "All of item added"), // 가득차있을경우
};

USTRUCT(BlueprintType)
struct FItemAddResult
{
	GENERATED_BODY()

	FItemAddResult() : 
	ActualAmountAdded(0),
	operationResult(EItemAddResult::IAR_NoItemAdded),
	ResultMessage(FText::GetEmpty())
	{}; // 함수 본문필요 x

	// Actual amount of item that was added to the Inventory 인벤토리에 추가된 항목의 실제 양
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	int32 ActualAmountAdded; // 우리가 원하는 필드에 실제 금액을 추가

	// Enum representing the end state of an add item operation  항목 추가 작업의 종료 상태를 나타내는 열거
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	EItemAddResult operationResult;

	// Informational message that can be passed with the result 결과와 함께 전달할 수 있는 정보 메시지
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	FText ResultMessage;

	static FItemAddResult AddedNone(const FText& ErrorText)// 새로운 항목추가 결과를 생성할 필요가 없다는것을 의미
	{
		FItemAddResult AddedNoneResult;
		AddedNoneResult.ActualAmountAdded = 0;
		AddedNoneResult.operationResult = EItemAddResult::IAR_NoItemAdded;
		AddedNoneResult.ResultMessage = ErrorText;
		return AddedNoneResult;
	}; 

	static FItemAddResult AddedPartial(const int32 PartialAmountAdded, const FText& ErrorText)
	{

	};
	
	static FItemAddResult AddedAll(const int32 AmountAdded, const FText& Message)
	{

	};

	
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MITHRILDUNGEON_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
};
