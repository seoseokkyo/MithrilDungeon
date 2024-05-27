// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "StateComponent.generated.h"

UENUM()
enum EStateType
{
	HP,
	SP
};

class ADungeonOrganism;

// DataTable(베이스 포인트)
USTRUCT(BlueprintType)
struct FCharacterStat : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxSP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Strength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Agility;
};

class UDataTable;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MITHRILDUNGEON_API UStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	FCharacterStat stat;

	float currentHP = 0.0f;
	float MaxHP = 0.0f;
	float currentSP = 0.0f;
	float MaxSP = 0.0f;
	int32 currentStrength = 0;
	int32 currentAgility = 0;

	ADungeonOrganism* character;

public:

	void InitStat();

	// Get Current Value
	float GetStatePoint(EStateType stateType);
	float AddStatePoint(EStateType stateType, float value);

	int32 GetStrength() { return currentStrength; };
	int32 GetAgility() { return currentAgility; };

	// Set CurrentValue
	void UpdateStat();
};
