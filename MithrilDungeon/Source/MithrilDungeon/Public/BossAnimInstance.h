// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enemy.h"
#include "Boss/Boss.h"
#include "BossAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MITHRILDUNGEON_API UBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MySettings")
	class ABoss* enemy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MySettings")
	EEnemyState currentState;

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Net)
	float Horizontal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Net)
	float Vertical;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Net)
	bool bDie = false;
};
