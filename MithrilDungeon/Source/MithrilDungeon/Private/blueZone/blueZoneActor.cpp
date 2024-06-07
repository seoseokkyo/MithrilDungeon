// Fill out your copyright notice in the Description page of Project Settings.


#include "blueZone/blueZoneActor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <../../../../../../../Source/Runtime/Core/Public/Math/UnrealMathUtility.h>
#include "MithrilDungeonCharacter.h"
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>

// Sets default values
AblueZoneActor::AblueZoneActor()
{
	PrimaryActorTick.bCanEverTick = true;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(RootComponent);

	meshComp->SetGenerateOverlapEvents(true);
	meshComp->SetWorldScale3D(FVector(75.0f));

}

void AblueZoneActor::BeginPlay()
{
	Super::BeginPlay();

	//meshComp->OnComponentBeginOverlap.AddDynamic(this, &AblueZoneActor::BeginOverlap);
	meshComp->SetWorldScale3D(FVector(75.0f));

	// 현재 남아있는 플레이어 검색
	for (TActorIterator<AMithrilDungeonCharacter> findActor(GetWorld()); findActor; ++findActor)
	{
		remainPlayers.Add(*findActor);
	}
}

void AblueZoneActor::Tick(float DeltaTime)
{
	// lerp x~x1 까지 alpha스피드로 이동
	Super::Tick(DeltaTime);
	circleshrinksTime += DeltaTime;
	if (circleshrinksTime >= 10.0f)
	{
		bblueZoneStart = true;
	}

	float Rad = 0.0f;

	if (bblueZoneStart)
	{
		blueZoneTime += DeltaTime;
		// 2분동안 원을 줄어들게 만들고 싶다.
		FVector three = FVector(FMath::Lerp(75, 0, blueZoneTime / 120.0f));
		Rad = three.X * 100 / 2;

		meshComp->SetWorldScale3D(FVector(three));
		if (blueZoneTime >= 120.0f)
		{
			circleshrinksTime = 0;
			bblueZoneStart = false;
		}

		TArray<FHitResult> hits;
		FCollisionObjectQueryParams params;

		params.AddObjectTypesToQuery(ECC_PhysicsBody);
		params.AddObjectTypesToQuery(ECC_Pawn);
		params.AddObjectTypesToQuery(ECC_Visibility);

		//GetWorld()->SweepMultiByObjectType(hits, GetActorLocation(), GetActorLocation(), FQuat::Identity, params, FCollisionShape::MakeSphere(Rad));

		GetWorld()->SweepMultiByChannel(hits, GetActorLocation(), GetActorLocation(), FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(Rad));

		UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("BlueZone Rad : %f"), Rad));

		for (auto remain : remainPlayers)
		{
			bool bFind = false;

			for (auto hit : hits)
			{
				auto pc = Cast<AMithrilDungeonCharacter>(hit.GetActor());
				if (pc != nullptr)
				{
					UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Hit Character : %s"), *pc->GetActorNameOrLabel()));
				}

				if (remain == hit.GetActor())
				{
					bFind = true;
					break;
				}
			}

			if (bFind == false)
			{
				auto pc = Cast<AMithrilDungeonCharacter>(remain);
				if (pc != nullptr)
				{
					pc->ServerRPC_AmountDamage(3* DeltaTime);

					//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Damaged By BlueZone : %s"), *pc->GetActorNameOrLabel()));
				}
			}
		}
	}
}

void AblueZoneActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 오버랩 된 사람이 플레이어라면
	Player = Cast<AMithrilDungeonCharacter>(OtherActor);
	UE_LOG(LogTemp, Warning, TEXT("player1"))
		if (OtherActor->IsA<AMithrilDungeonCharacter>())
		{
			bPlayer = true;
			UE_LOG(LogTemp, Warning, TEXT("player"));
			//데미지를 주는데 시간이 갈수록 주는 데미지가 강해져야함.
		}

}

