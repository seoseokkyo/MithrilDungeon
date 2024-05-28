// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonOrganism.h"
#include <Engine/DamageEvents.h>
#include <Kismet/KismetSystemLibrary.h>
#include "Animation/AnimMontage.h"
#include "BaseWeapon.h"
#include "CombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ADungeonOrganism::ADungeonOrganism()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	stateComp = CreateDefaultSubobject<UStateComponent>(TEXT("StateComp"));

	combatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComp"));

	characterName = TEXT("Default");
}

// Called when the game starts or when spawned
void ADungeonOrganism::BeginPlay()
{
	Super::BeginPlay();
		
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("characterName : %s"), *characterName));

	stateComp->InitStat();
	stateComp->UpdateStat();
}

// Called every frame
void ADungeonOrganism::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADungeonOrganism::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//float ADungeonOrganism::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
//{
//	// 맞는 방향으로 캐릭터 회전
//	FVector dir = DamageCauser->GetActorLocation() - GetActorLocation();
//
//	SetActorRotation(dir.GetSafeNormal().Rotation());
//
//	// State반영
//	if (stateComp != nullptr)
//	{
//		stateComp->AddStatePoint(HP, -DamageAmount);
//	}
//
//	// 히트 애니메이션 재생
//	if (hitReaction != nullptr)
//	{
//		PlayAnimMontage(hitReaction);
//	}
//
//	// 디버그
//	if (1)
//	{
//		if (EventInstigator != nullptr)
//		{
//			UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("EventInstigator : %s"), *EventInstigator->GetActorNameOrLabel()));
//		}
//
//		if (DamageCauser != nullptr)
//		{
//			UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Attack By : %s"), *DamageCauser->GetActorNameOrLabel()));
//		}
//	}
//
//	return 0.0f;
//}

void ADungeonOrganism::ContinueAttack_Implementation()
{
	combatComponent->bAttacking = false;

	if (combatComponent->bAttackSaved)
	{
		combatComponent->bAttackSaved = false;

		AttackEvent();
	}
}

void ADungeonOrganism::ResetCombat_Implementation()
{
	bToggleCombatAnimPlay = false;
	bOnAttack = false;
}

bool ADungeonOrganism::CanReceiveDamage_Implementation()
{
	return !bDead;
}

void ADungeonOrganism::AttackEvent()
{
	if (motionState == ECharacterMotionState::Idle)
	{
		PerformAttack(combatComponent->attackCount, false);
	}
}

void ADungeonOrganism::PerformAttack(int32 attackIndex, bool bUseRandom)
{
	ABaseWeapon* mainWeapon = nullptr;
	UAnimMontage* useMontage = nullptr;

	if (combatComponent)
	{
		mainWeapon = combatComponent->GetMainWeapon();

		if (mainWeapon != nullptr)
		{
			// 랜덤 인덱스 생성
			int32 montagesSize = mainWeapon->attackMontages.Num();
			int32 randIndex = FMath::RandRange(0, montagesSize - 1);

			useMontage = bUseRandom ? mainWeapon->attackMontages[attackIndex] : mainWeapon->attackMontages[randIndex];

			if (IsValid(useMontage))
			{
				combatComponent->bAttacking = true;
				motionState = ECharacterMotionState::Attack;

				//mainWeapon->weaponDamage = attackDamageArray[combatComponent->attackCount];

				mainWeapon->weaponDamage = 20;

				float attackAnimTime = PlayAnimMontage(useMontage);

				FTimerHandle handler;
				GetWorldTimerManager().SetTimer(handler, [&]() {

				combatComponent->bAttacking = false;
				motionState = ECharacterMotionState::Idle;

				// 카운트 증가
				combatComponent->attackCount++;

				int32 montageLastIndex = mainWeapon->attackMontages.Num() - 1;

				// 카운트 초기화
				if (combatComponent->attackCount > montageLastIndex)
				{
					combatComponent->attackCount = 0;
				}

				GetWorldTimerManager().ClearTimer(handler);

				}, 1.0f, false, attackAnimTime);
			}
		}
	}
}

void ADungeonOrganism::EnableRagdoll()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None, 0);

		if (GetCapsuleComponent())
		{
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

			FAttachmentTransformRules attachTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);

			if (GetMesh())
			{
				GetMesh()->SetCollisionProfileName(TEXT("ragdoll"));
				GetMesh()->SetAllBodiesBelowSimulatePhysics(pelvisBoneName, true, true);
				GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(pelvisBoneName, 1.0f, false, true);
			}

			//if (CameraBoom)
			//{
			//	if (GetMesh())
			//	{
			//		CameraBoom->AttachToComponent(GetMesh(), attachTransformRules, pelvisBoneName);
			//		CameraBoom->bDoCollisionTest = false;

			//		GetMesh()->SetCollisionProfileName(TEXT("ragdoll"));
			//		GetMesh()->SetAllBodiesBelowSimulatePhysics(pelvisBoneName, true, true);
			//		GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(pelvisBoneName, 1.0f, false, true);
			//	}
			//	else
			//	{
			//		UE_LOG(LogTemp, Warning, TEXT("AEromCharacter : %d"), __LINE__);
			//	}
			//}
			//else
			//{
			//	UE_LOG(LogTemp, Warning, TEXT("AEromCharacter : %d"), __LINE__);
			//}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AEromCharacter : %d"), __LINE__);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AEromCharacter : %d"), __LINE__);
	}
}
