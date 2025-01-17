// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstance_Interface.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */

 class ADungeonOrganism;

UCLASS()
class MITHRILDUNGEON_API UCharacterAnimInstance : public UAnimInstance, public IAnimInstance_Interface
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType weaponType = EWeaponType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCombatEnable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDie = false;

	UPROPERTY()
	ADungeonOrganism* myCharacter;

	virtual void UpdateCombatType_Implementation(EWeaponType eType) override;
	virtual void UpdateCombatEnabled_Implementation(bool bEnable) override;
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
