// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DungeonOrganism.h"
#include "Enemy.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	IDLE,		
	MOVE,		
	ATTACK,		
	ATTACKDELAY,	
	RETURN,		
	DAMAGED,		
	DIE			
};


UCLASS()
class MITHRILDUNGEON_API AEnemy : public ADungeonOrganism
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	EEnemyState enemyState = EEnemyState::IDLE;

	//UPROPERTY(EditAnywhere, Category = "MySettings")
	//class UStaticMeshComponent* swordComp;

	//UPROPERTY(EditAnywhere, Category = "MySettings")
	//class UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float traceSpeed = 450.0f;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float attackDistance = 170.0f;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float attackDelayTime = 1.5f;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float limitDistance = 1500.0f;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float returnSpeed = 500.0f;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	int32 EnemyMaxHP = 100.0f;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	int32 EnemyCurrentHP = EnemyMaxHP;

	void OnDamaged(int32 dmg);

	void SearchPlayer();

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UAnimMontage* attack_Montage;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UAnimMontage* death_Montage;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UAnimMontage* HitReact_Montage;

	// 상태
	void Idle();
	void MoveTotaget();
	void Attack();
	void AttackDelay();
	void Die();

	// 회전
	float rotTime = 0;
	FRotator rotStart;
	FRotator rotTarget;
	FVector targetLoc;
	FVector loctarget;
	FVector moveDir;   // 이동방향 잡기위한 변수
	bool bLookTarget = false;

	bool bAttack = false;

	bool bOnAttackDelay = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "weapon")
	TSubclassOf<class ABaseWeapon> defaultWeapon;


private:
	/*UPROPERTY()
	class UEnemyAnimInstance* anim;*/

	class AAIController* aiCon;

	UPROPERTY()
	class AMithrilDungeonCharacter* Player;

	UPROPERTY()
	class AActor* target;

	void PrintInfo();
};
