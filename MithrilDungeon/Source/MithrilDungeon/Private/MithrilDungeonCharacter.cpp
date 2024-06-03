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
#include "Interfaces/MithrilDungeonHUD.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WidgetComponent.h>
#include "StateComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Engine/EngineBaseTypes.h>
#include "DrawDebugHelpers.h" // 디버그라인
#include <../../../../../../../Source/Runtime/Core/Public/Math/UnrealMathUtility.h>

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

	InteractionCheckFrequecy = 0.1; // 빈도확인
	InteractionCheckDistance = 225.0f;  

	BaseEyeHeight = 74.0f; // 플레이어 눈 높이위로
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
		inventoryComp->SetWidget(nullptr);
		inventoryWidget->AddToViewport();
		inventoryWidget->inventoryOpen();
		inventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	HUD = Cast<AMithrilDungeonHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

void AMithrilDungeonCharacter::ServerRPC_ToggleCombat_Implementation()
{
	motionState = ECharacterMotionState::ToggleCombat;

	combatComponent->bCombatEnable = !combatComponent->bCombatEnable;

	NetMulticastRPC_ToggleCombat();
}

void AMithrilDungeonCharacter::NetMulticastRPC_ToggleCombat_Implementation()
{
	auto mainWeaponPtr = combatComponent->GetMainWeapon();

	float animPlayTime = 0.0f;

	if (!combatComponent->bCombatEnable)
	{
		if (mainWeaponPtr->exitCombatMontage)
		{
			animPlayTime = PlayAnimMontage(mainWeaponPtr->exitCombatMontage, 1.5f);

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
			animPlayTime = PlayAnimMontage(mainWeaponPtr->enterCombatMontage, 1.5f);

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


void AMithrilDungeonCharacter::PerformInteractionCheck()
{
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	FVector TraceStart {GetPawnViewLocation()}; // 명시적 초기화, 대괄호
	FVector TraceEnd {TraceStart + (GetViewRotation().Vector() * InteractionCheckDistance)}; // 마우스로 보기로 변경


	float LookDirection = FVector::DotProduct(GetActorForwardVector(), GetViewRotation().Vector()); // 내적함수

	if (LookDirection > 0)
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f,0, 2.0f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		FHitResult TraceHit; // 라인트레이스 결과 저장하는데 사용

		if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
		{
			if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
			{
				if (TraceHit.GetActor() != InteractionData.CurrentInteractable)
				{
					FoundInteractable(TraceHit.GetActor());
					return;
				}

				if (TraceHit.GetActor() == InteractionData.CurrentInteractable) 
				{
					return;
				}
			}
		}
	}

	NoInteractableFound();
}

void AMithrilDungeonCharacter::FoundInteractable(AActor* NewInteractable)
{
	if (IsInteracting()) // 캐릭터가 상호작용하는 경우 호출하고 싶은 기능
	{
		 EndInteract();
	}

	if (InteractionData.CurrentInteractable)// 상호작용 데이터가 있으면 현재 상호작용 가능하다고 알림
	{
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus(); // 동일하지않은 것이 나왔을때 이전 상호작용 가능 항목을 종료하는지 확인
	}

	// 상호작용 데이터를 현재 상호작용 가능 항목으로 가져오기 
	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;

	HUD->UpdateInteractionWidget(TargetInteractable->InteractableData); // 참조로 타겟데이터 전달

	TargetInteractable->BeginFocus();// 다음 대상 상호작용 가능 시작 호출
}

void AMithrilDungeonCharacter::NoInteractableFound()
{
	if (IsInteracting())// 확인하고 싶은것 X
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Interaction); // 타이머 초기화.
	}

	if (InteractionData.CurrentInteractable) // 현재 상호작용 가능 항목이 유효한 경우
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->EndFocus();
		} 

		HUD->HideInteractionWidget(); // 필요없는 위젯데이터 삭제

		InteractionData.CurrentInteractable = nullptr; // 현재 상호작용대상 nullptr
		TargetInteractable = nullptr; //대상 상호작용 가능항목 nullptr
	}
}

void AMithrilDungeonCharacter::BeginInteract()
{
	// 작용 가능한 상태에 아무것도 변경안되었는지 확인
	PerformInteractionCheck();

	if (InteractionData.CurrentInteractable) // 상호작용 데이터가 현재라면 프로세스로 돌아가게
	{
		if (IsValid(TargetInteractable.GetObject())) // 여전히 유효한경우
		{
			// 충돌이 발생하고 다시 상호작용 가능항목 유효한지 확인 후 상호작용 가능
			TargetInteractable->BeginInteract();

			if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f)) // 허용오차범위 0.1f, 0.2 0.3으로하면 문손잡이 돌릴때느낌으로 약간의 텀을 줄 수 있음.
			{
				 Interact();
			}
			else
			{
				GetWorld()->GetTimerManager().SetTimer(TimerHandle_Interaction,
				this, &AMithrilDungeonCharacter::Interact, TargetInteractable->InteractableData.InteractionDuration/*타겟 상호작용 가능*/,false);
			}
		}
	}
}

// 우리가 상호작용 하고있는지 확인필요 x
void AMithrilDungeonCharacter::EndInteract()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Interaction); // 타이머 초기화.

	if (IsValid(TargetInteractable.GetObject())) // 여전히 유효한경우
	{
		TargetInteractable->EndInteract();// 이제 대상 상호작용 가능, 대상 상호작용 종료
	}

}


void AMithrilDungeonCharacter::Interact()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Interaction); // 타이머 초기화.

	if (IsValid(TargetInteractable.GetObject())) // 여전히 유효한경우
	{
		TargetInteractable->Interact(this);// 이제 대상 상호작용 가능, 대상 상호작용 종료
	}

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

		// 물체 상호작용
		EnhancedInputComponent->BindAction(IA_Pressed, ETriggerEvent::Started, this, &AMithrilDungeonCharacter::BeginInteract);

		EnhancedInputComponent->BindAction(IA_Released, ETriggerEvent::Started, this, &AMithrilDungeonCharacter::EndInteract);
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
			UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("combatComponent->bCombatEnable : %s"), combatComponent->bCombatEnable ? TEXT("TRUE") : TEXT("FALSE")));

			ServerRPC_ToggleCombat();
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

	
	if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequecy)
	{
		PerformInteractionCheck();
	}


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
