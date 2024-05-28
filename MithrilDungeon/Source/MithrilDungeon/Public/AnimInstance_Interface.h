// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BaseWeapon.h"
#include "AnimInstance_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAnimInstance_Interface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MITHRILDUNGEON_API IAnimInstance_Interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateCombatType(E_WeaponType eType);

	virtual void UpdateCombatType_Implementation(E_WeaponType eType) = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateCombatEnable(bool bEnable);

	virtual void UpdateCombatEnable_Implementation(bool bEnable) = 0;
};
