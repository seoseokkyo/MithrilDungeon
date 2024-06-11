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
#include "Interfaces/MithrilDungeonHUD.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WidgetComponent.h>
#include "StateComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Engine/EngineBaseTypes.h>
#include "DrawDebugHelpers.h" // ����׶���
#include <../../../../../../../Source/Runtime/Core/Public/Math/UnrealMathUtility.h>
#include "World/Pickup.h"
#include "inventory/itemBase.h"
#include "Inventory/InventoryComponent.h"
#include "World/InterfaceTestActor.h"
#include "LootPanel.h"
#include <../../../../../../../Source/Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h>
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "Interfaces/InteractionInterface.h"
#include <../../../../../../../Source/Runtime/CoreUObject/Public/UObject/ScriptInterface.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>


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

	CreateInventory();

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	//FollowCamera->SetRelativeLocation(FVector(0, 0, 0));
	//FollowCamera->SetupAttachment(RootComponent);
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	//FollowCamera->bUsePawnControlRotation = true; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	characterName = TEXT("Player");

	InteractionCheckFrequecy = 0.1; // ��Ȯ��
	InteractionCheckDistance = 225.0f;

	BaseEyeHeight = 74.0f; // �÷��̾� �� ��������

	lootPanelWidget = CreateWidget<ULootPanel>(GetWorld(), ConstructorHelpers::FClassFinder<ULootPanel>(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Enemy/UserInterfaces/WBP_LootPanel.WBP_LootPanel_C'")).Class);
	//lootPanelWidget->SetVisibility(ESlateVisibility::Collapsed);

	int iTemp = 0;
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

	HUD = Cast<AMithrilDungeonHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	InterfaceActor = Cast<AInterfaceTestActor>(InterfaceActor);

	if (IsLocallyControlled())
	{
		InitRandomItem();
	}

	//PlayerInventory->HandleAddItem();
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

	//<< SSK �̰� �������� �׽�Ʈ�� �غ��� ��
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

	FVector TraceStart{ GetPawnViewLocation() }; // ����� �ʱ�ȭ, ���ȣ
	FVector TraceEnd{ TraceStart + (GetViewRotation().Vector() * InteractionCheckDistance) }; // ���콺�� ����� ����

	float LookDirection = FVector::DotProduct(GetActorForwardVector(), GetViewRotation().Vector()); // �����Լ�

	if (LookDirection > 0)
	{
		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		FHitResult TraceHit; // ����Ʈ���̽� ��� �����ϴµ� ���

		if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_PhysicsBody, QueryParams))
		{
			auto charCheck = Cast<ADungeonOrganism>(TraceHit.GetActor());
			if (charCheck != nullptr)
			{
				focusedChar = charCheck;
			}

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
		else
		{
			if (focusedChar != nullptr)
			{
				focusedChar = nullptr;
			}
		}
	}

	NoInteractableFound();
}

void AMithrilDungeonCharacter::FoundInteractable(AActor* NewInteractable)
{
	if (IsInteracting()) // ĳ���Ͱ� ��ȣ�ۿ��ϴ� ��� ȣ���ϰ� ���� ���
	{
		EndInteract();
	}

	if (InteractionData.CurrentInteractable)// ��ȣ�ۿ� �����Ͱ� ������ ���� ��ȣ�ۿ� �����ϴٰ� �˸�
	{
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus(); // ������������ ���� �������� ���� ��ȣ�ۿ� ���� �׸��� �����ϴ��� Ȯ��
	}

	// ��ȣ�ۿ� �����͸� ���� ��ȣ�ۿ� ���� �׸����� �������� 
	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;

	HUD->UpdateInteractionWidget(TargetInteractable->InteractableData); // ������ Ÿ�ٵ����� ����

	TargetInteractable->BeginFocus();// ���� ��� ��ȣ�ۿ� ���� ���� ȣ��
}

void AMithrilDungeonCharacter::NoInteractableFound()
{
	if (IsInteracting())// Ȯ���ϰ� ������ X
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Interaction); // Ÿ�̸� �ʱ�ȭ.
	}

	if (InteractionData.CurrentInteractable) // ���� ��ȣ�ۿ� ���� �׸��� ��ȿ�� ���
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->EndFocus();
		}

		HUD->HideInteractionWidget(); // �ʿ���� ���������� ����

		InteractionData.CurrentInteractable = nullptr; // ���� ��ȣ�ۿ��� nullptr
		TargetInteractable = nullptr; //��� ��ȣ�ۿ� �����׸� nullptr
	}
}

void AMithrilDungeonCharacter::BeginInteract()
{
	// �ۿ� ������ ���¿� �ƹ��͵� ����ȵǾ����� Ȯ��
	PerformInteractionCheck();

	if (focusedChar != nullptr)
	{
		auto contents = focusedChar->PlayerInventory->GetInventoryContents();
		focusedChar->LootByOthers(this);
	}
	else
	{
		ServerRPC_Interact();
	}
}

// �츮�� ��ȣ�ۿ� �ϰ��ִ��� Ȯ���ʿ� x
void AMithrilDungeonCharacter::EndInteract()
{

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Interaction); // Ÿ�̸� �ʱ�ȭ.

	if (IsValid(TargetInteractable.GetObject())) // ������ ��ȿ�Ѱ��
	{
		TargetInteractable->EndInteract();// ���� ��� ��ȣ�ۿ� ����, ��� ��ȣ�ۿ� ����
	}

	//lootPanelWidget->SetVisibility(ESlateVisibility::Collapsed);
	lootPanelWidget->RemoveFromParent();
}


void AMithrilDungeonCharacter::Interact()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Interaction); // Ÿ�̸� �ʱ�ȭ.

	if (IsValid(TargetInteractable.GetObject())) // ������ ��ȿ�Ѱ��
	{
		TargetInteractable->Interact(this);// ���� ��� ��ȣ�ۿ� ����, ��� ��ȣ�ۿ� ����
	}

}

void AMithrilDungeonCharacter::ServerRPC_Interact_Implementation()
{
	if (InteractionData.CurrentInteractable) // ��ȣ�ۿ� �����Ͱ� ������ ���μ����� ���ư���
	{
		if (IsValid(TargetInteractable.GetObject())) // ������ ��ȿ�Ѱ��
		{
			NetMulticastRPC_Interact(TargetInteractable);
		}
	}
}

void AMithrilDungeonCharacter::NetMulticastRPC_Interact_Implementation(const TScriptInterface<IInteractionInterface>& Interactable)
{
	// �浹�� �߻��ϰ� �ٽ� ��ȣ�ۿ� �����׸� ��ȿ���� Ȯ�� �� ��ȣ�ۿ� ����
	Interactable->BeginInteract();

	this->Interact();
}

float AMithrilDungeonCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return 0.0f;
}

void AMithrilDungeonCharacter::DieFunction()
{
	auto param = GetMesh()->GetCollisionResponseToChannels();
	param.SetResponse(ECC_Visibility, ECollisionResponse::ECR_Block);

	GetMesh()->SetCollisionResponseToChannels(param);

	if (IsLocallyControlled())
	{
		auto pc = Cast<APlayerController>(Controller);

		if (pc)
		{
			DisableInput(pc);
		}
	}

	motionState = ECharacterMotionState::Die;

	Super::DieFunction();
}

void AMithrilDungeonCharacter::CreateInventory()
{
	// Do Not Super Call
	if (PlayerInventory == nullptr)
	{
		PlayerInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("PlayerInventory"));
	}

	PlayerInventory->SetSlotsCapacity(60); //�κ��丮 ���� 20������
	PlayerInventory->SetWeightCapacity(50.0f); // ���Կ뷮 50����
}

void AMithrilDungeonCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMithrilDungeonCharacter, InteractionData);
	DOREPLIFETIME(AMithrilDungeonCharacter, TargetInteractable);
}

void AMithrilDungeonCharacter::UpdateInteractionWidget() const
{
	if (IsValid(TargetInteractable.GetObject()))
	{
		HUD->UpdateInteractionWidget(TargetInteractable->InteractableData); // ������(*)Ȯ���غ���
	}
}


void AMithrilDungeonCharacter::ToggleMenu()
{
	HUD->ToggleMenu();
}

void AMithrilDungeonCharacter::DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop)
{
	// �κ��丮 null�� �ƴ϶��
	if (PlayerInventory->FindMatchingItem(ItemToDrop))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		// ĳ���� 50�չ��⿡�� ������
		const FVector SpawnLocation{ GetActorLocation() + (GetActorForwardVector() * 50.0f) };

		const FTransform SpawnTransform(GetActorRotation(), SpawnLocation);

		// ��������
		const int32 RemoveQuantity = PlayerInventory->RemoveAmountOfItem(ItemToDrop, QuantityToDrop);

		APickup* Pickup = GetWorld()->SpawnActor<APickup>(APickup::StaticClass(), SpawnTransform, SpawnParams);

		Pickup->InitializeDrop(ItemToDrop, RemoveQuantity);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Item to drop was Some how null"));
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

		// �κ��丮 ����ݱ�
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &AMithrilDungeonCharacter::ToggleMenu);
		/*EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &AMithrilDungeonCharacter::InventoryOnOff);*/


		// ��ü ��ȣ�ۿ�
		EnhancedInputComponent->BindAction(IA_Pressed, ETriggerEvent::Started, this, &AMithrilDungeonCharacter::BeginInteract);


		EnhancedInputComponent->BindAction(IA_Pressed, ETriggerEvent::Completed, this, &AMithrilDungeonCharacter::EndInteract);
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

void AMithrilDungeonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequecy)
	{
		PerformInteractionCheck();
	}

	if (0)
	{
		PrintInfo();
	}
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
