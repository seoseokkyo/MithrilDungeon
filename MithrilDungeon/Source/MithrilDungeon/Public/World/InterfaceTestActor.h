// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "InterfaceTestActor.generated.h"

UCLASS()
class MITHRILDUNGEON_API AInterfaceTestActor : public AActor, public IInteractionInterface // �������̽� ���
{
	GENERATED_BODY()
	
public:
	AInterfaceTestActor();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Test Actor")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditInstanceOnly, Category = "Test Actor")
	FInteractableData InstanceInteractableData; // ��ȣ�ۿ� ������ �������� �ν��Ͻ��� �����ؾ���.


	// �������̽��� �����ϴ� �Լ� ������
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void BeginInteract() override;
	virtual void EndInteract() override;
	virtual void Interact(AMithrilDungeonCharacter* PlayerCharacter) override;

};
