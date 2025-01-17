// Fill out your copyright notice in the Description page of Project Settings.


#include "CollisionComponent.h"

// Sets default values for this component's properties
UCollisionComponent::UCollisionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	this->startSocketName = FName("Weapon Start");
	this->endSocketName = FName("Weapon End");
	this->traceRadius = 20;
	this->drawDebugType = EDrawDebugTrace::None;

	collisionObjectTypes.Reset();
	collisionObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	collisionObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
}


// Called when the game starts
void UCollisionComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bCollisionEnabled)
	{
		CollisionTrace();
	}
}

void UCollisionComponent::SetCollisionMesh(UPrimitiveComponent* meshComp)
{
	collisionMeshComponent = meshComp;
}

void UCollisionComponent::SetEnableCollision(bool bEnable)
{
	if (bEnable)
	{
		ClearHitActor();
	}

	bCollisionEnabled = bEnable;
}

void UCollisionComponent::ClearHitActor()
{
	alreadyHitActors.Reset();
}

void UCollisionComponent::CollisionTrace()
{
	FVector startSocketLocation = collisionMeshComponent->GetSocketLocation(startSocketName);
	FVector endSocketLocation = collisionMeshComponent->GetSocketLocation(endSocketName);

	TArray<FHitResult> arrayHits;

	UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), startSocketLocation, endSocketLocation, traceRadius, collisionObjectTypes, false, actorsToIgnore, drawDebugType, arrayHits, true, FLinearColor::Red, FLinearColor::Green, 1.0f);

	for (const FHitResult& elem : arrayHits)
	{
		lastHitStruct = elem;

		if (alreadyHitActors.Contains(lastHitStruct.GetActor()) == false)
		{
			hittedActor = lastHitStruct.GetActor();

			alreadyHitActors.Add(hittedActor);

			if (onHitDeligate.IsBound())
			{
				onHitDeligate.Execute(lastHitStruct);
			}
		}
	}
}

void UCollisionComponent::AddActorToIgnore(AActor* addActor)
{
	actorsToIgnore.AddUnique(addActor);
}

void UCollisionComponent::RemoveActorToIgnore(AActor* removeActor)
{
	actorsToIgnore.Remove(removeActor);
}