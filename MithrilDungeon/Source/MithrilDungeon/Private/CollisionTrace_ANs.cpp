// Fill out your copyright notice in the Description page of Project Settings.


#include "CollisionTrace_ANs.h"
#include "CombatComponent.h"
#include "BaseWeapon.h"
#include "CollisionComponent.h"


void UCollisionTrace_ANs::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp != nullptr)
	{
		AActor* owenrActor = MeshComp->GetOwner();
		if (owenrActor != nullptr)
		{

			auto comps = owenrActor->GetComponentsByTag(UCombatComponent::StaticClass(), FName(TEXT("CombatComp")));
			for (auto Comp : comps)
			{
				auto combatComp = Cast<UCombatComponent>(Comp);
				if (combatComp != nullptr)
				{
					ABaseWeapon* mainWeapon = combatComp->GetMainWeapon();

					if (IsValid(mainWeapon))
					{
						if (mainWeapon->collisionComponent != nullptr)
						{
							mainWeapon->collisionComponent->SetEnableCollision(true);	// Enable
						}
					}
				}
			}

			//UCombatComponent* combatComp = owenrActor->GetComponentByClass<UCombatComponent>();
			//if (combatComp != nullptr)
			//{
			//	ABaseWeapon* mainWeapon = combatComp->GetMainWeapon();
			//
			//	if (IsValid(mainWeapon))
			//	{
			//		if (mainWeapon->collisionComponent != nullptr)
			//		{
			//			mainWeapon->collisionComponent->SetEnableCollision(true);	// Enable
			//		}
			//	}
			//}
		}
	}
}

void UCollisionTrace_ANs::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp != nullptr)
	{
		AActor* owenrActor = MeshComp->GetOwner();
		if (owenrActor != nullptr)
		{
			auto comps = owenrActor->GetComponentsByTag(UCombatComponent::StaticClass(), FName(TEXT("CombatComp")));
			for (auto Comp : comps)
			{
				auto combatComp = Cast<UCombatComponent>(Comp);
				if (combatComp != nullptr)
				{
					ABaseWeapon* mainWeapon = combatComp->GetMainWeapon();

					if (IsValid(mainWeapon))
					{
						if (mainWeapon->collisionComponent != nullptr)
						{
							mainWeapon->collisionComponent->SetEnableCollision(false);	// Disable
						}
					}
				}
			}

			//UCombatComponent* combatComp = owenrActor->GetComponentByClass<UCombatComponent>();

			//if (combatComp != nullptr)
			//{
			//	ABaseWeapon* mainWeapon = combatComp->GetMainWeapon();

			//	if (IsValid(mainWeapon))
			//	{
			//		if (mainWeapon->collisionComponent != nullptr)
			//		{
			//			mainWeapon->collisionComponent->SetEnableCollision(false);	// Disable
			//		}
			//	}
			//}
		}
	}
}
