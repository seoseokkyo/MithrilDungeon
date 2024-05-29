// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class ABaseWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MITHRILDUNGEON_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	// Variables
	UPROPERTY(BlueprintReadWrite)
	ABaseWeapon*	mainWeapon;

	UPROPERTY(BlueprintReadWrite)
	bool			bCombatEnable = false;

	UPROPERTY(BlueprintReadWrite)
	int32			attackCount;

	UPROPERTY(BlueprintReadWrite)
	bool			bAttackSaved;

	UPROPERTY(BlueprintReadWrite)
	bool			bAttacking;

	// Functions
	void			SetMainWeapon(ABaseWeapon* baseWeapon);
	ABaseWeapon*	GetMainWeapon();
	void			SetCombatEnabled(bool bEnable);
	bool			CheckCombatEnabled();
	void			ResetAttack();
		
};