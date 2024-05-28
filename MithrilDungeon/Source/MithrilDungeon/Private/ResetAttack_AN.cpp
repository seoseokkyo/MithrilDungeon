// Fill out your copyright notice in the Description page of Project Settings.


#include "ResetAttack_AN.h"
#include "CombatInterface.h"

void UResetAttack_AN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp != nullptr)
	{
		ICombatInterface* combatInterface = Cast<ICombatInterface>(MeshComp->GetOwner());
		if (combatInterface != nullptr)
		{
			combatInterface->ResetAttack_Implementation();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s : %d"), *FString(__FILE__), __LINE__);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : %d"), *FString(__FILE__), __LINE__);
	}
}
