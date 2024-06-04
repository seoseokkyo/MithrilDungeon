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
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>


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
		if (bDead == false)
		{
			Die();
		}
		break;
	default:
		break;
	}

	// 공격전 회전
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

void AEnemy::Attack()
{
	// 공격전 플레이어에게로 방향회전
	FVector lookDir = Player->GetActorLocation() - GetActorLocation();
	moveDir = lookDir.GetSafeNormal();  //노멀라이즈를 해줘야 백터 길이가 1이됨.
	FRotator newRot = UKismetMathLibrary::MakeRotFromZX(GetActorUpVector(), moveDir);  // 첫번째 축은 고정축 두번째 축은 맞추려는 축
	rotStart = GetActorRotation();
	rotTarget = newRot;
	bLookTarget = true;

	/*UE_LOG(LogTemp, Warning, TEXT("Attack!!"));*/
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	bAttack = true;

	bOnAttackDelay = false;

	if (AnimInstance && bAttack)
	{
		PlayAnimMontage(attack_Montage);
		UE_LOG(LogTemp, Warning, TEXT("AttackAM!!"))
			enemyState = EEnemyState::ATTACKDELAY;
	}

}

void AEnemy::AttackDelay()
{
	if (bOnAttackDelay == false)
	{
		if (aiCon != nullptr)
		{
			aiCon->StopMovement();
		}

		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("AttackDelay Call"));

		FTimerHandle AttackTimer;

		bOnAttackDelay = true;

		GetWorld()->GetTimerManager().SetTimer(AttackTimer, [&]() {

			enemyState = EEnemyState::IDLE;

			UKismetSystemLibrary::PrintString(GetWorld(), TEXT("EEnemyState::IDLE"));

			}, 6, false);
	}
}

void AEnemy::DieFunction()
{
	GetMesh()->GetAnimInstance()->StopAllMontages(0.2);
	
	Super::DieFunction();

	ServerRPC_DieFunction();
}

void AEnemy::ServerRPC_DieFunction_Implementation()
{
	enemyState = EEnemyState::DIE;

	aiCon->SetFocus(nullptr);

	FTimerHandle hnd;
	GetWorldTimerManager().SetTimer(hnd, [&]() {

		EnableRagdoll();

		}, 2.0f, false);

	//NetMulticastRPC_DieFunction();
}

void AEnemy::NetMulticastRPC_DieFunction_Implementation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		float animTime = PlayAnimMontage(death_Montage);
		UE_LOG(LogTemp, Warning, TEXT("death_AM!!"));
	}
}

void AEnemy::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		PlayAnimMontage(death_Montage);
		UE_LOG(LogTemp, Warning, TEXT("death_AM!!"));
	}
}

void AEnemy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEnemy, enemyState);
}

void AEnemy::PrintInfo()
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



void AEnemy::OnDamaged(int32 dmg)
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

void AEnemy::SearchPlayer()
{
	//UE_LOG(LogTemp, Warning, TEXT("sug"));

	FVector Start = GetActorLocation();
	FVector End = Player->GetActorLocation();
	if (FVector::Distance(Start, End) > 1000)
	{
		enemyState = EEnemyState::IDLE;
	}
	else
	{
		enemyState = EEnemyState::MOVE;
	}
}



