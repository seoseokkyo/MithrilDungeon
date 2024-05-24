// Fill out your copyright notice in the Description page of Project Settings.


#include "portal/portalActor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>

// Sets default values
AportalActor::AportalActor()
{
	PrimaryActorTick.bCanEverTick = true;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(RootComponent);

	
	portalComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("portalComp"));
	portalComp->SetupAttachment(RootComponent);

	// ������ �ǰ� ����
	portalComp->SetGenerateOverlapEvents(true);

}

void AportalActor::BeginPlay()
{
	Super::BeginPlay();
	
	portalComp->SetVisibility(false);

	portalComp->OnComponentBeginOverlap.AddDynamic(this, &AportalActor::BeginOverlap);

}


void AportalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AportalActor::portalCreate()
{
	FTimerHandle portalCreateHandle;

	GetWorld()->GetTimerManager().SetTimer(portalCreateHandle, FTimerDelegate::CreateLambda([&](){

		meshComp->SetVisibility(false);
		UE_LOG(LogTemp, Warning, TEXT("portal"));
		portalComp->SetVisibility(true);

	}),8.0f,false); // ���ۿ����� 13��

}

void AportalActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// ������ �� ����� �÷��̾���
	// PortalActor = Cast<APortalActor>(OtherActor);
	//if (OtherActor->IsA(Player))
	//	{ 
	// �κ�� ������
	//UGameplayStatics::OpenLevel(this, "lobbyLevel");
	//	}
}

