// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"

void UCharacterAnimInstance::UpdateCombatType_Implementation(E_WeaponType eType)
{
	weaponType = eType;
}

void UCharacterAnimInstance::UpdateCombatEnable_Implementation(bool bEnable)
{
	bCombatEnable = bEnable;
}
