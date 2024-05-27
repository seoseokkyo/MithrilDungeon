// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "BaseWeapon.h"
#include <GameFramework/Character.h>

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UCombatComponent::SetMainWeapon(ABaseWeapon* baseWeapon)
{
	if (IsValid(mainWeapon))
	{
		mainWeapon->OnUnEquipped();
		mainWeapon->Destroy();
	}
	
	mainWeapon = baseWeapon;
}

ABaseWeapon* UCombatComponent::GetMainWeapon()
{
	return mainWeapon;
}

void UCombatComponent::SetCombatEnabled(bool bEnable)
{
	bCombatEnable = bEnable;

	/*
	*	1. GetOwner()
	*	2. CastToCharacter
	*	3. Character���� Mesh����
	*	4. Mesh���� GetAnimInstance
	*	5. GetAnimInstance�� IAnimInstance_Interface��ȯ�� ������ ��� UpdateCombatEnabledȣ��
	*/

	//Cast<IAnimInstance_Interface>(Cast<ACharacter>(GetOwner())->GetMesh()->GetAnimInstance())->UpdateCombatEnabled(bCombatEnable);
}

bool UCombatComponent::CheckCombatEnabled()
{
	return bCombatEnable;
}

void UCombatComponent::ResetAttack()
{
	attackCount = 0;
	bAttackSaved = false;
	bAttacking = false;
}

