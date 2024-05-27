// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Engine/DataTable.h>
#include "ParentItem.generated.h"

class UDataTable;

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Expendable,
	Equipmentable
};

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
