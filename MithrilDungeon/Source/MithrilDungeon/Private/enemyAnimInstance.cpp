// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyAnimInstance.h"

void UenemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	enemy = Cast<AEnemy>(GetOwningActor());
}

void UenemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (enemy != nullptr)
	{
		currentState = enemy->enemyState;
	}

	if (enemy == nullptr) return;

	// Me�� �̵��ӵ��� Horizontal, Vertical�� �и��ؼ� �� �� ������ ä���ʹ�.

	// enemy�� �̵��ӵ��� ��������ʹ�.
	auto vel = enemy->GetVelocity();  // vel �� �ִ���̴� �ƽ���ũ���ǵ��� 600�� �ɰ���.
	// enemy�� �̵� �ӵ��� enemy�� �չ����� Dot �ؼ� Vertical�� �����ϰ�ʹ�.
	Vertical = FVector::DotProduct(vel, enemy->GetActorForwardVector());

	// enemy�� �̵� �ӵ��� enemy�� �����ʹ����� Dot �ؼ� Horizontal�� �����ϰ�ʹ�.
	Horizontal = FVector::DotProduct(vel, enemy->GetActorRightVector());
}
