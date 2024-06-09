// Fill out your copyright notice in the Description page of Project Settings.


#include "portal/portalActor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "MithrilDungeonCharacter.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SphereComponent.h>


// Sets default values
AportalActor::AportalActor()
{
	PrimaryActorTick.bCanEverTick = true;

	portalComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("portalComp"));
	portalComp->SetupAttachment(RootComponent);

	// 오버랩 되게 설정
	portalComp->SetGenerateOverlapEvents(true);


	

}

void AportalActor::BeginPlay()
{
	Super::BeginPlay();

	portalComp->OnComponentBeginOverlap.AddDynamic(this, &AportalActor::BeginOverlap);

}


void AportalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}


void AportalActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 오버랩 된 사람이 플레이어라면
	Player = Cast<AMithrilDungeonCharacter>(OtherActor);
	if (OtherActor->IsA<AMithrilDungeonCharacter>())
	{
		// 로비로 나가기
		//UGameplayStatics::OpenLevel(this, "lobbyLevel");
		// Destroy();
		UE_LOG(LogTemp, Warning, TEXT("Level"));
	}
}

