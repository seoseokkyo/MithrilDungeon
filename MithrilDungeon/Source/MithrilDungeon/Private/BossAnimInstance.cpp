// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAnimInstance.h"

void UBossAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	enemy = Cast<ABoss>(GetOwningActor());
}

void UBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (enemy != nullptr)
	{
		currentState = enemy->enemyState;
	}	

	if (enemy == nullptr) return;

	bDie = enemy->bDead;

	// Me의 이동속도를 Horizontal, Vertical로 분리해서 각 각 변수에 채우고싶다.

	// enemy의 이동속도를 가져오고싶다.
	auto vel = enemy->GetVelocity();  // vel 의 최대길이는 맥스워크스피드의 600이 될거임.
	// enemy의 이동 속도와 enemy의 앞방향을 Dot 해서 Vertical에 대입하고싶다.
	Vertical = FVector::DotProduct(vel, enemy->GetActorForwardVector());

	// enemy의 이동 속도와 enemy의 오른쪽방향을 Dot 해서 Horizontal에 대입하고싶다.
	Horizontal = FVector::DotProduct(vel, enemy->GetActorRightVector());
}
