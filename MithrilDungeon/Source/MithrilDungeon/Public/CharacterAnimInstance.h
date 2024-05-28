// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstance_Interface.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MITHRILDUNGEON_API UCharacterAnimInstance : public UAnimInstance, public IAnimInstance_Interface
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	E_WeaponType weaponType = E_WeaponType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCombatEnable = false;

	virtual void UpdateCombatType_Implementation(E_WeaponType eType) override;
	virtual void UpdateCombatEnable_Implementation(bool bEnable) override;
};
