// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Boss.h"
#include "AIController.h"
#include "MithrilDungeonCharacter.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "NavigationInvokerComponent.h"
#include "EngineUtils.h"
#include "Animation/AnimMontage.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Animation/AnimInstance.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>
#include <../../../../../../../Source/Runtime/Core/Public/Math/UnrealMathUtility.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>


ABoss::ABoss()
{
	PrimaryActorTick.bCanEverTick = true;

	// ����
	swordComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordComp"));
	swordComp->SetupAttachment(GetMesh(), TEXT("b_RightHandSocket"));
	swordComp->SetRelativeLocation(FVector(11.577815, -3.876722, 5.406297));
	swordComp->SetRelativeRotation(FRotator(36.225576, 100.412465, -48.868406));

	// ���⳯, �������Ǹ� ������
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	boxComp->SetupAttachment(swordComp);
	boxComp->SetRelativeLocation(FVector(-0.000001, 0, 82.601549));
	boxComp->SetRelativeScale3D(FVector(0.1, 0.1, 2.2));

	characterName = TEXT("Skeleton");
}


void ABoss::BeginPlay()
{
	Super::BeginPlay();

	// ���忡 �ִ� �÷��̾ ã�´�.
	for (TActorIterator<AMithrilDungeonCharacter> findActor(GetWorld()); findActor; ++findActor)
	{
		Player = *findActor;
	}

	aiCon = GetController<AAIController>();

	// �⺻ ���¸� IDLE ���·� �ʱ�ȭ�Ѵ�.
	enemyState = EBoseState::IDLE;

}

void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (enemyState)
	{
	case EBoseState::IDLE:
		Idle();
		break;
	case EBoseState::MOVE:
		MoveTotaget();
		break;
	case EBoseState::ATTACK:
		Attack();
		break;
	case EBoseState::ATTACKDELAY:
		AttackDelay();
		break;
	case EBoseState::RETURN:

		break;
	case EBoseState::DAMAGED:

		break;
	case EBoseState::DIE:
		Die();
		break;
	default:
		break;
	}

	// ������ ȸ��
	if (bLookTarget)
	{
		if (rotTime < 1.0f)
		{
			rotTime += DeltaTime;
			SetActorRotation(FMath::Lerp(rotStart, rotTarget, rotTime));
			//UE_LOG(LogTemp, Warning, TEXT("111111111111111"));
		}
		else
		{
			rotTime = 0;
			targetLoc = GetActorLocation() + GetActorForwardVector() * 1500;
			bLookTarget = false;
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("State Transition: %s"), *StaticEnum<EEnemyState>()->GetValueAsString(enemyState));
}

void ABoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABoss::Idle()
{
	SearchPlayer();


}

void ABoss::MoveTotaget()
{
	targetLoc = Player->GetActorLocation();
	if (aiCon != nullptr)
	{
		if (FVector::Distance(GetActorLocation(), targetLoc) < limitDistance && FVector::Distance(GetActorLocation(), targetLoc) > attackDistance)
		{
			UE_LOG(LogTemp, Warning, TEXT("move!!"));
			aiCon->MoveToActor(Player);
		}
		if (FVector::Distance(GetActorLocation(), targetLoc) <= attackDistance)
		{
			enemyState = EBoseState::ATTACK;
		}
	}
}

void ABoss::Attack()
{
	// ������ �÷��̾�Է� ����ȸ��
	FVector lookDir = Player->GetActorLocation() - GetActorLocation();
	moveDir = lookDir.GetSafeNormal();  //��ֶ���� ����� ���� ���̰� 1�̵�.
	FRotator newRot = UKismetMathLibrary::MakeRotFromZX(GetActorUpVector(), moveDir);  // ù��° ���� ������ �ι�° ���� ���߷��� ��
	rotStart = GetActorRotation();
	rotTarget = newRot;
	bLookTarget = true;

	/*UE_LOG(LogTemp, Warning, TEXT("Attack!!"));*/
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	bAttack = true;
	if (AnimInstance && bAttack)
	{
		PlayAnimMontage(attack_Montage);
		UE_LOG(LogTemp, Warning, TEXT("AttackAM!!"))
			enemyState = EBoseState::ATTACKDELAY;
	}

}

void ABoss::AttackDelay()
{
	aiCon->StopMovement();

	FTimerHandle AttackTimer;
	GetWorld()->GetTimerManager().SetTimer(AttackTimer, [&]() {
		enemyState = EBoseState::IDLE;
		}, 3, false);
}

void ABoss::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		PlayAnimMontage(death_Montage);
		UE_LOG(LogTemp, Warning, TEXT("death_AM!!"))
	}
}



void ABoss::OnDamaged(int32 dmg)
{
	EnemyCurrentHP = FMath::Clamp(EnemyCurrentHP - dmg, 0, 100);
	if (EnemyCurrentHP <= 0)
	{
		enemyState = EBoseState::DIE;


	}
	else
	{
		enemyState = EBoseState::DAMAGED;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			PlayAnimMontage(HitReact_Montage);
			UE_LOG(LogTemp, Warning, TEXT("HitReactAM!!"))
		}
	}

}

void ABoss::SearchPlayer()
{
	//UE_LOG(LogTemp, Warning, TEXT("sug"));

	FVector Start = GetActorLocation();
	FVector End = Player->GetActorLocation();
	if (FVector::Distance(Start, End) < 100)
	{
		enemyState = EBoseState::IDLE;
	}
	else
	{
		enemyState = EBoseState::MOVE;
	}
}
