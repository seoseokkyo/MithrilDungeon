// Fill out your copyright notice in the Description page of Project Settings.


#include "CollisionTrace_ANs.h"
#include "CombatComponent.h"
#include "BaseWeapon.h"
#include "CollisionComponent.h"

void UCollisionTrace_ANs::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	TSubclassOf<UCombatComponent> combatComp;
	auto compCheck = Cast<UCombatComponent>(MeshComp->GetOwner()->GetComponentByClass(combatComp));

	if (compCheck)
	{
		ABaseWeapon* mainWeapon = compCheck->GetMainWeapon();
		if (mainWeapon != nullptr)
		{
			mainWeapon->collisionComponent->SetEnableCollision(true);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UCollisionTrace_ANs::NotifyBegin - NullptrErr"));
	}
}

void UCollisionTrace_ANs::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	TSubclassOf<UCombatComponent> combatComp;
	auto compCheck = Cast<UCombatComponent>(MeshComp->GetOwner()->GetComponentByClass(combatComp));

	if (compCheck)
	{
		ABaseWeapon* mainWeapon = compCheck->GetMainWeapon();
		if (mainWeapon != nullptr)
		{
			mainWeapon->collisionComponent->SetEnableCollision(false);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UCollisionTrace_ANs::NotifyEnd - NullptrErr"));
	}
}

