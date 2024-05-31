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

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(RootComponent);
	//meshComp->SetupAttachment(RootComponent);

	portalComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("portalComp"));
	portalComp->SetupAttachment(RootComponent);
	portalComp->SetRelativeLocation(FVector(0, 0, 300));

	// ������ �ǰ� ����
	portalComp->SetGenerateOverlapEvents(true);


}

void AportalActor::BeginPlay()
{
	Super::BeginPlay();

	//portalComp->SetVisibility(false);

	portalComp->OnComponentBeginOverlap.AddDynamic(this, &AportalActor::BeginOverlap);

}


void AportalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AportalActor::portalCreate()
{
	FTimerHandle portalCreateHandle;

	GetWorld()->GetTimerManager().SetTimer(portalCreateHandle, FTimerDelegate::CreateLambda([&]() {

		meshComp->SetVisibility(false);
		UE_LOG(LogTemp, Warning, TEXT("portal"));
		portalComp->SetVisibility(true);

		}), 8.0f, false); // ���ۿ����� 13��
}

void AportalActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// ������ �� ����� �÷��̾���
	Player = Cast<AMithrilDungeonCharacter>(OtherActor);
	if (OtherActor->IsA<AMithrilDungeonCharacter>())
	{
		// �κ�� ������
		//UGameplayStatics::OpenLevel(this, "lobbyLevel");
		UE_LOG(LogTemp, Warning, TEXT("Level"));
	}
}

