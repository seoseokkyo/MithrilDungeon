// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
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
#include "BaseWeapon.h"


// Sets default values
AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	//// 무기
	//swordComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordComp"));
	//swordComp->SetupAttachment(GetMesh(), TEXT("b_RightHandSocket"));
	//swordComp->SetRelativeLocation(FVector(11.577815, -3.876722, 5.406297));
	//swordComp->SetRelativeRotation(FRotator(36.225576, 100.412465, -48.868406));

	//// 무기날, 오버랩되면 데미지
	//boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	//boxComp->SetupAttachment(swordComp);
	//boxComp->SetRelativeLocation(FVector(-0.000001, 0, 82.601549));
	//boxComp->SetRelativeScale3D(FVector(0.1,0.1,2.2));

	characterName = TEXT("Skeleton");
}


void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	// 월드에 있는 플레이어를 찾는다.
	for (TActorIterator<AMithrilDungeonCharacter> findActor(GetWorld()); findActor; ++findActor)
	{
		Player = *findActor;
	}

	aiCon = GetController<AAIController>();

	// 기본 상태를 IDLE 상태로 초기화한다.
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
			equipment->OnEquipped();
		}
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	//UE_LOG(LogTemp, Warning, TEXT("State Transition: %s"), *StaticEnum<EEnemyState>()->GetValueAsString(enemyState));
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::Idle()
{
	SearchPlayer();
	

}

void AEnemy::MoveTotaget()
{
	FVector targetLoc = Player->GetActorLocation();
	if (aiCon != nullptr)
	{
		if (FVector::Distance(GetActorLocation(), targetLoc) < limitDistance && FVector::Distance(GetActorLocation(),targetLoc) > attackDistance)
		{ 
			UE_LOG(LogTemp, Warning, TEXT("move!!"));
			aiCon->MoveToActor(Player);
		}
		if (FVector::Distance(GetActorLocation(), targetLoc) <= attackDistance)
		{
			enemyState = EEnemyState::ATTACK;
		}
	}
}

void AEnemy::Attack()
{
	/*UE_LOG(LogTemp, Warning, TEXT("Attack!!"));*/
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
 	if (AnimInstance && !bAttack)
 	{
		PlayAnimMontage(attack_Montage);
		UE_LOG(LogTemp, Warning, TEXT("AttackAM!!"))
		bAttack = true;
	}
	
}

void AEnemy::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		PlayAnimMontage(death_Montage);
		UE_LOG(LogTemp, Warning, TEXT("death_AM!!"))
	}
}



void AEnemy::OnDamaged(int32 dmg)
{
	EnemyCurrentHP = FMath::Clamp(EnemyCurrentHP -dmg, 0,100);
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

void AEnemy::SearchPlayer()
{
	//UE_LOG(LogTemp, Warning, TEXT("sug"));

	FVector Start = GetActorLocation();
	FVector End = Player->GetActorLocation();
	if (FVector::Distance(Start, End) < 100)
	{
		enemyState = EEnemyState::IDLE;
	}
	else
	{ 
		enemyState = EEnemyState::MOVE;
	}
}



