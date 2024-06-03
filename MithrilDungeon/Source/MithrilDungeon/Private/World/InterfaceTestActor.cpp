// Fill out your copyright notice in the Description page of Project Settings.


#include "World/InterfaceTestActor.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AInterfaceTestActor::AInterfaceTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

}

// Called when the game starts or when spawned
void AInterfaceTestActor::BeginPlay()
{
	Super::BeginPlay();
	
	InteractableData = InstanceInteractableData; // 인스턴스 데이터와 같게설정
}

// Called every frame
void AInterfaceTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInterfaceTestActor::BeginFocus()
{
	if (Mesh)
	{
		Mesh->SetRenderCustomDepth(true); // 정의깊이 랜더링 부울 입력값 설정
	}
}

void AInterfaceTestActor::EndFocus()
{
	if (Mesh)
	{
		Mesh->SetRenderCustomDepth(false); // 정의깊이 랜더링 부울 입력값 설정
	}
}

void AInterfaceTestActor::BeginInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("Calling BeginInteract override on Interface test actor")); // 시작 상호작용 재정의 호출

}

void AInterfaceTestActor::EndInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("Calling EndInteract override on Interface test actor"));
}

void AInterfaceTestActor::Interact(AMithrilDungeonCharacter* PlayerCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("Calling Interact override on Interface test actor"));
}

