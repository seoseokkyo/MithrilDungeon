// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include <GameFramework/Character.h>
#include <Kismet/GameplayStatics.h>
#include "CollisionComponent.h"
#include "CombatComponent.h"

ABaseWeapon::ABaseWeapon()
{
	collisionComponent = CreateDefaultSubobject<UCollisionComponent>(TEXT("CollisionComponent"));
	ItemStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	if (collisionComponent != nullptr)
	{
		collisionComponent->onHitDeligate.BindUObject(this, &ABaseWeapon::OnHitCollisionComponent);
	}
}

void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseWeapon::OnEquipped()
{
	//Super::OnEquipped();
	if (GetOwner() != nullptr)
	{
		combatComponent = GetOwner()->GetComponentByClass<UCombatComponent>();
		if (combatComponent != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("handSocketName : %s"), *handSocketName.ToString());
			UE_LOG(LogTemp, Warning, TEXT("AttachSocketName : %s"), *AttachSocketName.ToString());

			FName selectedName = (combatComponent->bCombatEnable ? handSocketName : AttachSocketName);

			UE_LOG(LogTemp, Warning, TEXT("CombatComponent.bCombatEnable : %d"), combatComponent->bCombatEnable);
			UE_LOG(LogTemp, Warning, TEXT("selectedName : %s"), *selectedName.ToString());

			AttachActor(selectedName);

			combatComponent->SetMainWeapon(this);

			ACharacter* characterPtr = Cast<ACharacter>(GetOwner());
			if (characterPtr)
			{
				USkeletalMeshComponent* skeletalMeshComponent = characterPtr->GetMesh();
				if (skeletalMeshComponent)
				{
					auto animInstance = skeletalMeshComponent->GetAnimClass();

					//if (animInstance->ImplementsInterface(UAnimInstance_Interface::StaticClass()))
					//{
					//	IAnimInstance_Interface::Execute_UpdateCombatType(animInstance, eWeaponType);
					//
					//	UE_LOG(LogTemp, Warning, TEXT("%s Send UUpdateCombatType"), *animInstance->GetName());
					//}
					//else
					//{
					//	UE_LOG(LogTemp, Warning, TEXT("%s Is do not Have IAnimInstance_Interface"), *skeletalMeshComponent->GetName());
					//}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("skeletalMeshComponent Is Nullptr"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Owner Is Not Character"));
			}

			collisionComponent->SetCollisionMesh(GetItemMesh());
			collisionComponent->AddActorToIgnore(GetOwner());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("selectedName : %p"), this);
	}
}

void ABaseWeapon::OnHitCollisionComponent(FHitResult lastHitStruct)
{
	UE_LOG(LogTemp, Warning, TEXT("OnHitCollisionComponent Called"));

	AActor* hitActor = lastHitStruct.GetActor();

	FVector hitFromDirection = GetOwner()->GetActorForwardVector();
	TSubclassOf<UDamageType> damageTypeClass = {};
	
	//<< 이거 damageTypeClass랑 블루프린트의 기본값이랑 차이가 있는지 확인필요
	UGameplayStatics::ApplyPointDamage(hitActor, weaponDamage, hitFromDirection, lastHitStruct, GetInstigatorController(), this, damageTypeClass);

	//if (Cast<ICombat_Interface>(hitActor)->CanReceiveDamage())
	//{
	//	/*
	//		 AActor* DamagedActor, 
	//		 float BaseDamage, 
	//		 FVector const& HitFromDirection, 
	//		 FHitResult const& HitInfo, 
	//		 AController* EventInstigator, 
	//		 AActor* DamageCauser, 
	//		 TSubclassOf<UDamageType> DamageTypeClass
	//	*/

	//	FVector hitFromDirection = GetOwner()->GetActorForwardVector();
	//	TSubclassOf<UDamageType> damageTypeClass = {};

	//	//<< 이거 damageTypeClass랑 블루프린트의 기본값이랑 차이가 있는지 확인필요
	//	UGameplayStatics::ApplyPointDamage(hitActor, weaponDamage, hitFromDirection, lastHitStruct, GetInstigatorController(), this, damageTypeClass);
	//}
}

void ABaseWeapon::SimulateWeaponPhysics()
{
	GetItemMesh()->SetCollisionProfileName(FName("PhysicsActor"));
	GetItemMesh()->SetSimulatePhysics(true);
}
