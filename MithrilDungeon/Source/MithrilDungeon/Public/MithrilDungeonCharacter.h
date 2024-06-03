// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "DungeonOrganism.h"
#include "Interfaces/InteractionInterface.h" // cpp가아닌 헤더에 둬야함.
#include "MithrilDungeonCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UCombatComponent;
class AMithrilDungeonHUD;

UENUM()
enum class MyEnum : int8
{
	NM_Standalone,
	NM_DedicatedServer,
	NM_ListenServer,
	NM_Client,
	NM_MAX,
};

// 진원s
USTRUCT()
struct FInteractionData
{	
	GENERATED_USTRUCT_BODY()

	FInteractionData() : CurrentInteractable(nullptr), LastInteractionCheckTime(0.0f)
	{

	}

	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY()
	float LastInteractionCheckTime; // 상호작용 가능항목 포함, 마지막 상호작용 포함, 시간확인

};
// 진원e

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AMithrilDungeonCharacter : public ADungeonOrganism
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ToggleCombatAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InventoryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Pressed;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Released;

public:
	AMithrilDungeonCharacter();

	UPROPERTY(EditDefaultsOnly, Category = TPS)
	class UWidgetComponent* inventoryComp;

	class UinventoryWidget* inventoryWidget;

	// 진원 S
	FORCEINLINE bool IsInteracting() const {return GetWorld()->GetTimerManager().IsTimerActive(TimerHandle_Interaction); }; // 현재 상호작용중인지 아닌지

	// 진원 E
protected:
	// 진원 S
	UPROPERTY()
	AMithrilDungeonHUD* HUD;

	// 진원 E
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void CharacterJump(const FInputActionValue& Value);

	void LightAttackFunction(const FInputActionValue& Value);

	void ToggleCombatFunction(const FInputActionValue& Value);

	void InventoryOnOff(const FInputActionValue& Value);
	bool bInventorystate = false;

	virtual void Tick(float DeltaTime) override;

	void PrintInfo();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	// 진원 s
	UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
	TScriptInterface<IInteractionInterface> TargetInteractable;

	float InteractionCheckFrequecy;

	float InteractionCheckDistance; // 추적이 캐릭터에서 얼마나 멀리 발사될지

	FTimerHandle TimerHandle_Interaction; 

	FInteractionData InteractionData;

	void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable); // 상호작용 가능항목호출, 새 상호작용 가능항목 가져오기
	void NoInteractableFound();// 하지만 찾지못하면 호출X
	void BeginInteract();
	void EndInteract();
	void Interact();
	// 진원 e
		
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "weapon")
	TSubclassOf<class ABaseWeapon> defaultWeapon;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(Server, Reliable)
	void ServerRPC_ToggleCombat();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_ToggleCombat();
};

