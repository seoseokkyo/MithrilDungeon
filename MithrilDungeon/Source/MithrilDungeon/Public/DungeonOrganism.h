// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CombatInterface.h"
#include "StateComponent.h"
#include "DungeonOrganism.generated.h"

UENUM()
enum class ECharacterType : int8
{
	PlayerCharacter,
	Enemy
};

UENUM()
enum class ECharacterMotionState : int8
{
	Idle,
	Attack,
	Defence,
	//Hit,
	Jump,
	Die
};

class UAnimMontage;

UCLASS()
class MITHRILDUNGEON_API ADungeonOrganism : public ACharacter, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADungeonOrganism();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FString characterName;

	UStateComponent* stateComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	UAnimMontage* hitReaction;	

	ECharacterMotionState motionState = ECharacterMotionState::Idle;

public:
	ECharacterType characterType;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	// 이건 그냥 캐릭터마다 만드는게 속 편하겠다
	void Attack();

	FString GetName() {return characterName;};
};
