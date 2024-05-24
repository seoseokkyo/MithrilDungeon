// Fill out your copyright notice in the Description page of Project Settings.


#include "blueZone/blueZoneActor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>

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
	
}

void AblueZoneActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

}

void AblueZoneActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 오버랩 된 사람이 플레이어라면
	// PortalActor = Cast<APortalActor>(OtherActor);
	//if (OtherActor->IsA(Player))
	//	{ 
	//	데미지를 주는데 시간이 갈수록 주는 데미지가 강해져야함.
	//	}

}

