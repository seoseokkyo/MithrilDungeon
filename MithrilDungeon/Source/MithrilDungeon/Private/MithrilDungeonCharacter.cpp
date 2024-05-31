// Copyright Epic Games, Inc. All Rights Reserved.

#include "MithrilDungeonCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "MithrilDungeonGameInstance.h"
#include "BaseWeapon.h"
#include "CombatComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include "inventory/inventoryWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WidgetComponent.h>
#include "StateComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Engine/EngineBaseTypes.h>

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AMithrilDungeonCharacter

AMithrilDungeonCharacter::AMithrilDungeonCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	//FollowCamera->SetRelativeLocation(FVector(0, 0, 0));
	//FollowCamera->SetupAttachment(RootComponent);
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	//FollowCamera->bUsePawnControlRotation = true; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	characterName = TEXT("Player");


	inventoryComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPComp"));
	inventoryComp->SetupAttachment(RootComponent);
}

void AMithrilDungeonCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

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
	
	inventoryWidget = Cast<UinventoryWidget>(inventoryComp->GetWidget());

	if (inventoryWidget != nullptr)
	{
		inventoryWidget->inventoryOpen();
		inventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	}	
}

void AMithrilDungeonCharacter::ServerRPC_PlayAnimationMontage_Implementation(class UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{	
	animPlayTime = PlayAnimMontage(AnimMontage, InPlayRate, StartSectionName);

	NetMulticastRPC_PlayAnimationMontage(AnimMontage, InPlayRate, StartSectionName);
}

void AMithrilDungeonCharacter::NetMulticastRPC_PlayAnimationMontage_Implementation(class UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
	animPlayTime = PlayAnimMontage(AnimMontage, InPlayRate, StartSectionName);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMithrilDungeonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMithrilDungeonCharacter::CharacterJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMithrilDungeonCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMithrilDungeonCharacter::Look);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AMithrilDungeonCharacter::LightAttackFunction);

		EnhancedInputComponent->BindAction(ToggleCombatAction, ETriggerEvent::Started, this, &AMithrilDungeonCharacter::ToggleCombatFunction);

		// 인벤토리 열고닫기
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &AMithrilDungeonCharacter::InventoryOnOff);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMithrilDungeonCharacter::CharacterJump(const FInputActionValue& Value)
{
	if (motionState != ECharacterMotionState::Idle)
	{
		return;
	}

	Super::Jump();
}

void AMithrilDungeonCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMithrilDungeonCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMithrilDungeonCharacter::LightAttackFunction(const FInputActionValue& Value)
{
	if (false == combatComponent->bCombatEnable)
	{
		return;
	}

	if (combatComponent->bAttacking)
	{
		combatComponent->bAttackSaved = true;
	}
	else
	{
		AttackEvent();
	}
}

void AMithrilDungeonCharacter::ToggleCombatFunction(const FInputActionValue& Value)
{
	auto mainWeaponPtr = combatComponent->GetMainWeapon();
	if (IsValid(mainWeaponPtr))
	{
		if (motionState == ECharacterMotionState::Idle)
		{
			motionState = ECharacterMotionState::ToggleCombat;

			UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("combatComponent->bCombatEnable : %s"), combatComponent->bCombatEnable ? TEXT("TRUE") : TEXT("FALSE")));

			combatComponent->bCombatEnable = !combatComponent->bCombatEnable;

			if (!combatComponent->bCombatEnable)
			{
				if (mainWeaponPtr->exitCombatMontage)
				{
					ServerRPC_PlayAnimationMontage(mainWeaponPtr->exitCombatMontage, 1.5f);

					UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("animPlayTime : %f"), animPlayTime));
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("ToggleCombatFunction : %d"), __LINE__);
				}
			}
			else
			{
				if (mainWeaponPtr->enterCombatMontage)
				{
					ServerRPC_PlayAnimationMontage(mainWeaponPtr->enterCombatMontage, 1.5f);

					UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("animPlayTime : %f"), animPlayTime));
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("ToggleCombatFunction : %d"), __LINE__);
				}
			}

			//<< SSK 이거 먹히는지 테스트는 해봐야 됨
			FTimerHandle timerHandle;

			GetWorldTimerManager().SetTimer(timerHandle, [&]()
			{
				motionState = ECharacterMotionState::Idle;

				GetWorld()->GetTimerManager().ClearTimer(timerHandle);

				UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("combatComponent->bCombatEnable : %s"), combatComponent->bCombatEnable ? TEXT("TRUE") : TEXT("FALSE")));

			}, animPlayTime, false, 1.0f);
		}
	}
}

void AMithrilDungeonCharacter::InventoryOnOff(const FInputActionValue& Value)
{
	// 인벤토리 온으로 변경
	if (!bInventorystate)
	{
		if (inventoryWidget != nullptr)
		{
			inventoryWidget->SetVisibility(ESlateVisibility::Visible);
			bInventorystate = true;
		}
	}

	// 인벤토리 오프로 변경
	else
	{
		if (inventoryWidget != nullptr)
		{ 
			inventoryWidget->SetVisibility(ESlateVisibility::Hidden);
			bInventorystate = false;
		}
	}

}

void AMithrilDungeonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PrintInfo();
}

void AMithrilDungeonCharacter::PrintInfo()
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

	FString str = FString::Printf(TEXT("localRole : %s\nremoteRole : %s\nowner : %s\nnetConn : %s\nnetMode : %s\nhasController : %s\n HP : %s\n SP : %s"), *localRole, *remoteRole, *owner, *netConn, *netMode, *hasController, *strHP, *strSP);

	FVector loc = GetActorLocation() + FVector(0, 0, 50);
	DrawDebugString(GetWorld(), loc, str, nullptr, FColor::White, 0, true);
}
