// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

#include "Mover.h"
#include "GameFramework/Actor.h"

UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType,	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* Actor = GetAcceptableActor();
	if(!Actor)
	{
		Mover->SetShouldMove(false);		
	}
	else
	{
		if(UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent()))
		{
			Component->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
			Component->SetSimulatePhysics(false);
		}
		Mover->SetShouldMove(true);
	}
}

void UTriggerComponent::SetMover(UMover* InMover)
{
	this->Mover = InMover;
}


AActor* UTriggerComponent::GetAcceptableActor() const
{
	TArray<AActor*>Actors;
	GetOverlappingActors(Actors);

	for (AActor* Actor : Actors)
	{
		bool bHasAcceptableTag = Actor->ActorHasTag(AcceptableActorTag);
		bool bIsGrabbed = Actor->ActorHasTag("Grabbed");

		if (bHasAcceptableTag && !bIsGrabbed)
		{
			return Actor;
		}
	}
	return nullptr;
}

