// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "blueZoneActor.generated.h"

UCLASS()
class MITHRILDUNGEON_API AblueZoneActor : public AActor
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;


public:	
	AblueZoneActor();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	class UStaticMeshComponent* meshComp;

	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	float circleshrinksTime = 0.0f;
	float blueZoneTime = 0.0f;

	bool bblueZoneStart = false;
	bool bPlayer = false;

	UPROPERTY(EditAnywhere, Category = "Mysettings")
	class AMithrilDungeonCharacter* Player;

};
