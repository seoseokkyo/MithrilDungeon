// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonOrganism.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Engine/DamageEvents.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include "CombatComponent.h"

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

float ADungeonOrganism::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// 맞는 방향으로 캐릭터 회전
	FVector dir = DamageCauser->GetActorLocation() - GetActorLocation();

	SetActorRotation(dir.GetSafeNormal().Rotation());

	// State반영
	if (stateComp != nullptr)
	{
		stateComp->AddStatePoint(HP, -DamageAmount);
	}

	// 히트 애니메이션 재생
	if (hitReaction != nullptr)
	{
		PlayAnimMontage(hitReaction);
	}

	// 디버그
	if (1)
	{
		if (EventInstigator != nullptr)
		{
			UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("EventInstigator : %s"), *EventInstigator->GetActorNameOrLabel()));
		}

		if (DamageCauser != nullptr)
		{
			UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Attack By : %s"), *DamageCauser->GetActorNameOrLabel()));
		}
	}

	return 0.0f;
}

void ADungeonOrganism::Attack()
{
	AActor* target = nullptr;

	FHitResult Hit;
	float calcedDamage = 10.0f;

	FPointDamageEvent DamageEvent(calcedDamage, Hit, GetActorForwardVector(), UDamageType::StaticClass());

	target->TakeDamage(calcedDamage, DamageEvent, Controller, this);
}
