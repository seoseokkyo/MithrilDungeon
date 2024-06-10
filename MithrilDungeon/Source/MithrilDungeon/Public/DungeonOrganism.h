// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CombatInterface.h"
#include "StateComponent.h"
#include "inventory/InventoryComponent.h"
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
	ToggleCombat,
	Attack,
	Defence,
	//Hit,
	Jump,
	Die
};

class UAnimMontage;
class UCombatComponent;
class ULootPanel;

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString characterName;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	bool bDead;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	UStateComponent* stateComp;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Components")
	UCombatComponent* combatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	ECharacterType characterType;
	
	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	UAnimMontage* hitReaction;	

	UPROPERTY(Replicated, BlueprintReadWrite)
	ECharacterMotionState motionState = ECharacterMotionState::Idle;

	
	UPROPERTY(VisibleAnywhere, Category = "Character | Inventory")
	UInventoryComponent* PlayerInventory;

	virtual void ContinueAttack_Implementation() override;
	virtual void ResetCombat_Implementation() override;
	virtual bool CanReceiveDamage_Implementation() override;

	TArray<int32> attackDamageArray;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	FName pelvisBoneName = TEXT("Pelvis");

	UFUNCTION(BlueprintCallable)
	void AttackEvent();

	UFUNCTION(BlueprintCallable)
	void PerformAttack(int32 attackIndex, bool bUseRandom);

	void EnableRagdoll();

	FString GetName() {return characterName;};

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void ServerRPC_PerformAttack(UAnimMontage* useMontage);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PerformAttack(UAnimMontage* useMontage);

	UFUNCTION(Server, Reliable)
	void ServerRPC_AmountDamage(float damage);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_AmountDamage(float currentHP);

	UFUNCTION()
	virtual void DieFunction();


	
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<ULootPanel> lootPanelWidgetClass;
		
	UPROPERTY()
	ULootPanel* lootPanelWidget;	 

	FORCEINLINE UInventoryComponent* GetInventory() const { return PlayerInventory; }; // �κ��丮 ��������

	void LootByOthers(ADungeonOrganism* otherCharacter);

	virtual void CreateInventory();

	void InitRandomItem();


	UPROPERTY(Replicated)
	int32 itemRandNums = 0;

	UPROPERTY(Replicated)
	int32 randItemIndex = 0;

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetItemRandNums(int32 randNums);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_SetItemRandNums(int32 randNums);

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetItemRandIndex(int32 randIndex);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_SetItemRandIndex(int32 randIndex);

	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnItem(int32 itemIndex);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_SpawnItem(int32 randIndex);
};
