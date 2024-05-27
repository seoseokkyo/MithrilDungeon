// Fill out your copyright notice in the Description page of Project Settings.


#include "blueZone/blueZoneActor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <../../../../../../../Source/Runtime/Core/Public/Math/UnrealMathUtility.h>
#include "MithrilDungeonCharacter.h"

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

	meshComp->OnComponentBeginOverlap.AddDynamic(this, &AblueZoneActor::BeginOverlap);
	meshComp->SetWorldScale3D(FVector(75.0f));
}

void AblueZoneActor::Tick(float DeltaTime)
{
	// lerp x~x1 ���� alpha���ǵ�� �̵�
	Super::Tick(DeltaTime);
	circleshrinksTime += DeltaTime;
	if (circleshrinksTime >= 10.0f)
	{
		bblueZoneStart = true;
	}

	if (bblueZoneStart)
	{ 
		blueZoneTime += DeltaTime;
		// 2�е��� ���� �پ��� ����� �ʹ�.
		FVector three = FVector (FMath::Lerp(75, 0, blueZoneTime / 120.0f));
		meshComp->SetWorldScale3D(FVector(three));
		if (blueZoneTime >= 120.0f)
		{
			circleshrinksTime = 0;
			bblueZoneStart = false;
		}
	}
	
}

void AblueZoneActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// ������ �� ����� �÷��̾���
	Player = Cast<AMithrilDungeonCharacter>(OtherActor);
	UE_LOG(LogTemp, Warning, TEXT("player1"))
	if (OtherActor->IsA<AMithrilDungeonCharacter>())
	{ 
		bPlayer = true;
		UE_LOG(LogTemp,Warning,TEXT("player"))
		//�������� �ִµ� �ð��� ������ �ִ� �������� ����������.
	}

}

