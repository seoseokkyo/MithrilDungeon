// Fill out your copyright notice in the Description page of Project Settings.


#include "StateComponent.h"
#include "DungeonOrganism.h"
#include "MithrilDungeonGameInstance.h"

// Sets default values for this component's properties
UStateComponent::UStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	character = GetOwner<ADungeonOrganism>();
}


// Called when the game starts
void UStateComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	UpdateStat();
}


// Called every frame
void UStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStateComponent::InitStat()
{
	if (GetOwner() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetOwner Is Null : %s, %d"), __FILE__, __LINE__);
		return;
	}

	auto gi = GetOwner()->GetGameInstance<UMithrilDungeonGameInstance>();
	if (gi != nullptr)
	{
		FCharacterStat characterStat = gi->GetCharacterDataTable(character->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GetGameInstance Is Null"));
	}
}

float UStateComponent::GetStatePoint(EStateType stateType)
{
	switch (stateType)
	{
	case HP:
		return currentHP;
	case SP:
		return currentSP;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Type Error, %s, %d"), __FILE__, __LINE__);
		break;
	}

	return 0.0f;
}

float UStateComponent::AddStatePoint(EStateType stateType, float value)
{
	switch (stateType)
	{
	case HP:
		return currentHP += value;
	case SP:
		return currentSP += value;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Type Error, %s, %d"), __FILE__, __LINE__);
		break;
	}

	return 0.0f;
}

void UStateComponent::UpdateStat()
{
	// �̰� ���� �ϴ� ���������̺� �ϳ� ����
	//auto characterCheck = Cast<ADungeonOrganism>(GetOwner());
	//if (characterCheck != nullptr)
	//{
	//	if (characterCheck->characterType == ECharacterType::PlayerCharacter)
	//	{

	//	}
	//}


	// ���� ��� �ִ� ������ ���⿡ �߰������ ��
	stat.MaxHP = MaxHP;
	stat.MaxSP = MaxSP;
}

