// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ResetAttack_AN.generated.h"

/**
 * 
 */
UCLASS()
class MITHRILDUNGEON_API UResetAttack_AN : public UAnimNotify
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
