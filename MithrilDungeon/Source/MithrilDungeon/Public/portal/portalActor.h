// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "portalActor.generated.h"

UCLASS()
class MITHRILDUNGEON_API AportalActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AportalActor();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* portalComp;

	void portalCreate();

	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly)
	class AMithrilDungeonCharacter* Player;

protected:
	virtual void BeginPlay() override;


	

};
