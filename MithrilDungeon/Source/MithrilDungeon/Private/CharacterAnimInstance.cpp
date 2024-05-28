// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"

void UCharacterAnimInstance::UpdateCombatType_Implementation(EWeaponType eType)
{
	weaponType = eType;
}

void UCharacterAnimInstance::UpdateCombatEnabled_Implementation(bool bEnable)
{
	bCombatEnable = bEnable;
}
