// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabberComponent.h"

#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Math/Rotator.h"
#include "CollisionShape.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Containers/Array.h"

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
	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent()) 
	{
		FVector TargetLocation = GetComponentLocation() + (GetForwardVector() * HoldDistance);
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
}

void UGrabberComponent::Grab()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr)
	{
		return;
	}

	FHitResult HitResult;
	bool bHasHit = GetGrabbableInReach(HitResult);

	if (bHasHit) {
		float LineDistance = FVector::Distance(HitResult.Location, HitResult.ImpactPoint);
		UE_LOG(LogTemp, Warning, TEXT("Length between ImpactPoint and Location: %f"), LineDistance);
		DrawDebugSphere(GetWorld(), HitResult.Location, 10, 10, FColor::Green, false, 5);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Red,false,5);

		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		HitComponent->SetSimulatePhysics(true);
		HitComponent->WakeAllRigidBodies();
		AActor* HitActor = HitResult.GetActor();
		HitActor->Tags.Add("Grabbed");
		HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitComponent, 
			NAME_None,
			HitResult.ImpactPoint, 
			GetComponentRotation()
		);
	}
}

void UGrabberComponent::Release()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();	
	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		auto GrabbedActor = PhysicsHandle->GetGrabbedComponent()->GetOwner();
		GrabbedActor->Tags.Remove("Grabbed");
		PhysicsHandle->ReleaseComponent();
	}
}

bool UGrabberComponent::GetGrabbableInReach(FHitResult& OutHitResult) const
{
	FVector LineStart = GetComponentLocation();
	FVector LineEnd = GetComponentLocation() + (GetForwardVector() * MaxDistance);
	DrawDebugLine(GetWorld(), LineStart, LineEnd, FColor::Red, true);
	DrawDebugSphere(GetWorld(), LineEnd, 10, 10, FColor::Blue, false, 5);

	FCollisionShape SphereShape = FCollisionShape::MakeSphere(GrabRadius);	

	return GetWorld()->SweepSingleByChannel(
		OutHitResult,
		LineStart,
		LineEnd,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		SphereShape
	);
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



