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
#include "BaseWeapon.h"
#include "CombatComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>


ABoss::ABoss()
{
	PrimaryActorTick.bCanEverTick = true;

	//// ����
	//swordComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordComp"));
	//swordComp->SetupAttachment(GetMesh(), TEXT("b_RightHandSocket"));
	//swordComp->SetRelativeLocation(FVector(11.577815, -3.876722, 5.406297));
	//swordComp->SetRelativeRotation(FRotator(36.225576, 100.412465, -48.868406));

	//// ���⳯, �������Ǹ� ������
	//boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	//boxComp->SetupAttachment(swordComp);
	//boxComp->SetRelativeLocation(FVector(-0.000001, 0, 82.601549));
	//boxComp->SetRelativeScale3D(FVector(0.1, 0.1, 2.2));

	characterName = TEXT("Skeleton");

	combatComponent_Additional = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComp_Additional"));
}


void ABoss::BeginPlay()
{
	Super::BeginPlay();

	// ���忡 �ִ� �÷��̾ ã�´�.
	//for (TActorIterator<AMithrilDungeonCharacter> findActor(GetWorld()); findActor; ++findActor)
	//{
	//	Player = *findActor;
	//}

	SpawnDefaultController();
	aiCon = GetController<AAIController>();

	// �⺻ ���¸� IDLE ���·� �ʱ�ȭ�Ѵ�.
	enemyState = EEnemyState::IDLE;

	if (characterName == TEXT("Skeleton"))
	{
		FActorSpawnParameters spawnParam;
		spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		spawnParam.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
		spawnParam.Owner = this;
		spawnParam.Instigator = this;

		ABaseWeapon* equipment = GetWorld()->SpawnActor<ABaseWeapon>(defaultWeapon, GetActorTransform(), spawnParam);

		if (equipment)
		{
			equipment->OnEquippedTarget(combatComponent);

			equipment->GetItemMesh()->SetVisibility(false);
			equipment->GetItemMesh()->bHiddenInGame = true;

			combatComponent->SetCombatEnabled(true);
		}

		ABaseWeapon* equipment_Additional = GetWorld()->SpawnActor<ABaseWeapon>(defaultWeapon_Additional, GetActorTransform(), spawnParam);

		if (equipment_Additional)
		{
			equipment_Additional->OnEquippedTarget(combatComponent_Additional);

			equipment_Additional->GetItemMesh()->SetVisibility(false);
			equipment_Additional->GetItemMesh()->bHiddenInGame = true;

			combatComponent_Additional->SetCombatEnabled(true);
		}
	}

}

void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PrintInfo();

	if (bDead)
	{
		if (enemyState != EEnemyState::DIE)
		{
			enemyState = EEnemyState::DIE;
		}

		return;
	}

	switch (enemyState)
	{
	case EEnemyState::IDLE:
		Idle();
		break;
	case EEnemyState::MOVE:
		MoveTotaget();
		break;
	case EEnemyState::ATTACK:
		Attack();
		break;
	case EEnemyState::ATTACKDELAY:
		AttackDelay();
		break;
	case EEnemyState::RETURN:

		break;
	case EEnemyState::DAMAGED:

		break;
	case EEnemyState::DIE:
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
			aiCon->SetFocus(Player);
			aiCon->MoveToActor(Player);
		}
		if (FVector::Distance(GetActorLocation(), targetLoc) <= attackDistance)
		{
			enemyState = EEnemyState::ATTACK;
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

	bOnAttackDelay = false;

	if (AnimInstance && bAttack)
	{
		AttackEvent();

		UE_LOG(LogTemp, Warning, TEXT("AttackAM!!"))
			enemyState = EEnemyState::ATTACKDELAY;
	}
}

void ABoss::AttackDelay()
{
	aiCon->StopMovement();

	FTimerHandle AttackTimer;
	GetWorld()->GetTimerManager().SetTimer(AttackTimer, [&]() {
		enemyState = EEnemyState::IDLE;
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

void ABoss::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABoss, combatComponent_Additional);
	DOREPLIFETIME(ABoss, enemyState);
}



void ABoss::OnDamaged(int32 dmg)
{
	EnemyCurrentHP = FMath::Clamp(EnemyCurrentHP - dmg, 0, 100);
	if (EnemyCurrentHP <= 0)
	{
		enemyState = EEnemyState::DIE;


	}
	else
	{
		enemyState = EEnemyState::DAMAGED;
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

	Player = nullptr;

	float nearestDist = 9999999.9f;

	// ���忡 �ִ� �÷��̾ ã�´�.
	for (TActorIterator<AMithrilDungeonCharacter> findActor(GetWorld()); findActor; ++findActor)
	{
		AMithrilDungeonCharacter* temp = nullptr;

		temp = *findActor;

		FVector Start = GetActorLocation();
		FVector End = temp->GetActorLocation();

		float dist = FVector::Dist(Start, End);

		if (dist < 1000)
		{
			if (nearestDist > dist)
			{
				nearestDist = dist;

				Player = *findActor;
			}
		}
	}

	if (Player == nullptr)
	{
		enemyState = EEnemyState::IDLE;
	}
	else
	{
		enemyState = EEnemyState::MOVE;
	}
}

void ABoss::PrintInfo()
{
	// localRole
	FString localRole = UEnum::GetValueAsString(GetLocalRole());

	// remoteRole
	FString remoteRole = UEnum::GetValueAsString(GetRemoteRole());

	// owner
	FString owner = GetOwner() ? GetOwner()->GetName() : "";

	// netConn
	FString netConn = GetNetConnection() ? "Valid" : "Invalid";

	FString netMode = UEnum::GetValueAsString((MyEnum)GetNetMode());

	FString hasController = Controller ? TEXT("HasController") : TEXT("NoController");

	FString strHP = FString::Printf(TEXT("%f"), stateComp->GetStatePoint(EStateType::HP));
	FString strSP = FString::Printf(TEXT("%f"), stateComp->GetStatePoint(EStateType::SP));

	FString strState = UEnum::GetValueAsString(enemyState);

	FString str = FString::Printf(TEXT("localRole : %s\nremoteRole : %s\nowner : %s\nnetConn : %s\nnetMode : %s\nhasController : %s\n HP : %s\n SP : %s\n strState : %s"), *localRole, *remoteRole, *owner, *netConn, *netMode, *hasController, *strHP, *strSP, *strState);

	FVector loc = GetActorLocation() + FVector(0, 0, 50);
	DrawDebugString(GetWorld(), loc, str, nullptr, FColor::White, 0, true);
}
