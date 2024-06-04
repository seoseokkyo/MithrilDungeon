// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"
#include "DungeonOrganism.h"

void UCharacterAnimInstance::UpdateCombatType_Implementation(EWeaponType eType)
{
	weaponType = eType;
}

void UCharacterAnimInstance::UpdateCombatEnabled_Implementation(bool bEnable)
{
	bCombatEnable = bEnable;
}

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	myCharacter = Cast<ADungeonOrganism>(GetOwningActor());
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (myCharacter != nullptr)
	{
		bDie = myCharacter->bDead;
	}
}

