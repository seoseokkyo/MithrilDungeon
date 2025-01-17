// Fill out your copyright notice in the Description page of Project Settings.


#include "blueZone/blueZoneActor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <../../../../../../../Source/Runtime/Core/Public/Math/UnrealMathUtility.h>
#include "MithrilDungeonCharacter.h"
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>

// Sets default values
AblueZoneActor::AblueZoneActor()
{
	PrimaryActorTick.bCanEverTick = true;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(RootComponent);

	meshComp->SetGenerateOverlapEvents(true);
}

void AblueZoneActor::BeginPlay()
{
	Super::BeginPlay();

	// 현재 남아있는 플레이어 검색
	for (TActorIterator<AMithrilDungeonCharacter> findActor(GetWorld()); findActor; ++findActor)
	{
		remainPlayers.Add(*findActor);
	}

	SettedScale = GetActorScale();
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
		FVector three = FVector(FMath::Lerp(SettedScale.X, 0, blueZoneTime / 120.0f));
		Rad = three.X * 50.0f;

		meshComp->SetWorldScale3D(FVector(three));
		if (blueZoneTime >= 120.0f)
		{
			circleshrinksTime = 0;
			bblueZoneStart = false;
		}

		for (const auto& remain : remainPlayers)
		{
			if (IsValid(remain))
			{
				FVector playerLoc = remain->GetActorLocation();
				FVector bluezoneLoc = GetActorLocation();

				float dist = FVector::Dist(playerLoc, bluezoneLoc);

				bool bCheck = false;

				if (playerLoc.X <= bluezoneLoc.X - Rad && playerLoc.X >= bluezoneLoc.X + Rad)
				{
					UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("InSize X")));
					bCheck = true;
				}

				if (playerLoc.Y <= bluezoneLoc.Y - Rad && playerLoc.Y >= bluezoneLoc.Y + Rad)
				{
					UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("InSize Y")));
					bCheck = true;
				}

				if (Rad < dist && bCheck == false)
				{
					auto pc = Cast<AMithrilDungeonCharacter>(remain);
					if (pc != nullptr)
					{
						pc->ServerRPC_AmountDamage(3 * DeltaTime);

						if (blueZoneSound)
						{
							if (blueZoneTime - lastPlay > 1)
							{
								lastPlay = blueZoneTime;

								UGameplayStatics::PlaySoundAtLocation(GetWorld(), blueZoneSound, pc->GetActorLocation());
							}

						}
					}
				}
			}
		}

		//TArray<FHitResult> hits;

		// Sweep 너무느림
		//GetWorld()->SweepMultiByChannel(hits, GetActorLocation(), GetActorLocation(), FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(Rad));

		//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("BlueZone Rad : %f"), Rad));

		// Sweep
	//	for (const auto& remain : remainPlayers)
	//	{
	//		bool bFind = false;

	//		for (const auto& hit : hits)
	//		{
	//			//auto pc = Cast<AMithrilDungeonCharacter>(hit.GetActor());
	//			//if (pc != nullptr)
	//			//{
	//			//	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Hit Character : %s"), *pc->GetActorNameOrLabel()));
	//			//}

	//			if (remain == hit.GetActor())
	//			{
	//				bFind = true;
	//				break;
	//			}
	//		}

	//		if (bFind == false)
	//		{
	//			auto pc = Cast<AMithrilDungeonCharacter>(remain);
	//			if (pc != nullptr)
	//			{
	//				pc->ServerRPC_AmountDamage(3* DeltaTime);

	//				//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Damaged By BlueZone : %s"), *pc->GetActorNameOrLabel()));
	//			}
	//		}
	//	}

	}
}
