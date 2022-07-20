// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabberComponent.h"

#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Math/Rotator.h"
#include "CollisionShape.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values for this component's properties
UGrabberComponent::UGrabberComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabberComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
		
}


// Called every frame
void UGrabberComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr)
	{
		return;
	}

	FVector TargetLocation = GetComponentLocation() + (GetForwardVector() * HoldDistance);
	PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
}

void UGrabberComponent::Grab()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr)
	{
		return;
	}

	FVector LineStart = GetComponentLocation();
	FVector LineEnd = GetComponentLocation() + (GetForwardVector() * MaxDistance);	
	DrawDebugLine(GetWorld(), LineStart, LineEnd, FColor::Red, true);
	DrawDebugSphere(GetWorld(), LineEnd, 10, 10, FColor::Blue, false, 5);

	FCollisionShape SphereShape = FCollisionShape::MakeSphere(GrabRadius);
	FHitResult HitResult;

	bool bHasHit = GetWorld()->SweepSingleByChannel(
		HitResult, 
		LineStart, 
		LineEnd, 
		FQuat::Identity,
		ECC_GameTraceChannel2,
		SphereShape
	);

	if (bHasHit) {
		float LineDistance = FVector::Distance(HitResult.Location, HitResult.ImpactPoint);
		UE_LOG(LogTemp, Warning, TEXT("Length between ImpactPoint and Location: %f"), LineDistance);
		DrawDebugSphere(GetWorld(), HitResult.Location, 10, 10, FColor::Green, false, 5);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Red,false,5);
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitResult.GetComponent(), 
			NAME_None,
			HitResult.ImpactPoint, 
			GetComponentRotation()
		);
	}
}

void UGrabberComponent::Release()
{
	//UE_LOG(LogTemp, Display, TEXT("Released"));
}

UPhysicsHandleComponent* UGrabberComponent::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (Result == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Grabber requires a UPhysicsComponent. "));
	}
	return Result;
}



