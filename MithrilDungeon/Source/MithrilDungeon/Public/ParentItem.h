// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Engine/DataTable.h>
#include "ParentItem.generated.h"

class UDataTable;

UENUM(BlueprintType)
enum class EItemType : uint8	// ������ ����, ����, ���� , ����Ʈ������ ��
{
	Expendable,
	Equipmentable,
	Armor UMETA(DisplayName = "Armor"),
	Weapon UMETA(DisplayName = "Weapon"),
	Shield UMETA(DisplayName = "Shield"),
	Spell UMETA(DisplayName = "Spell"),
	Consumable UMETA(DisplayName = "Consumable"),
	Quest UMETA(DisplayName = "Quest"),
	Mundane UMETA(DisplayName = "Mundane")

};

// HJW
UENUM()
enum class EItemQuality : uint8 // ǰ��, ���
{
	Shoddy UMETA(DisplayName = "Shoddy"),
	Common UMETA(DisplayName = "Common"),
	Quality UMETA(DisplayName = "Quality"),
	Masterwork UMETA(DisplayName = "Masterwork"),
	Grandmaster UMETA(DisplayName = "Grandmaster")
};

//UENUM()
//enum class EItemTypes : uint8 // ������ ����, ����, ���� , ����Ʈ������ ��
//{
//	/*Armor UMETA(DisplayName = "Armor"),
//	Weapon UMETA(DisplayName = "Weapon"),
//	Shield UMETA(DisplayName = "Shield"),
//	Spell UMETA(DisplayName = "Spell"),
//	Consumable UMETA(DisplayName = "Consumable"),
//	Quest UMETA(DisplayName = "Quest"),
//	Mundane UMETA(DisplayName = "Mundane")*/
//};

USTRUCT()
struct FItemStatistics // ���ݿ� ���Ұ�.
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere) // ��𼭵� �����ϰ����
	float ArmorRating; // �� ���

	UPROPERTY(EditAnywhere)
	float DamageValue; // ������ ��

	UPROPERTY(EditAnywhere)
	float RestorationAmount; // ���ర�� �Ҹ�ǰ

	UPROPERTY(EditAnywhere)
	float SellValue;
};

USTRUCT()
struct  FItemTextData // �ؽ��� ������
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FText Name;

	UPROPERTY(EditAnywhere)
	FText Description;

	UPROPERTY(EditAnywhere)
	FText InteractionText;

	UPROPERTY(EditAnywhere)
	FText UsageText;

};


USTRUCT()
struct FItemNumericData // ���ڵ����� ���Ե� 
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere)
	float Weight;

	UPROPERTY(EditAnywhere)
	int32 MaxStackSize;

	UPROPERTY(EditAnywhere)
	bool bisStackable;

};

USTRUCT()
struct FItemAssetData // ���� ������, ������ �������� ���尰���� �ֱ⵵ ����
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;

};

// ����ü
USTRUCT()
struct  FItemData : public FTableRowBase // �� ���̽� ���
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FName ID;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemQuality ItemQuality;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemStatistics ItemStatistics;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemTextData TextData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemNumericData NumericData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemAssetData AssetData;

};
// HJW

USTRUCT(BlueprintType)
struct FParentItemInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	FString itemImagePath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	EItemType itemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	int32 numberOfItemsOwned;
};

UCLASS()
class MITHRILDUNGEON_API AParentItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AParentItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FString itemName = TEXT("Default");

	FParentItemInfo itemInfo;
};
