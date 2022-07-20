// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

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

	TArray<AActor*>Actors;
	GetOverlappingActors(Actors);

	for (const AActor* Actor : Actors)
	{
		auto FirstActorName = Actor->GetActorNameOrLabel();
		UE_LOG(LogTemp, Warning, TEXT("First overlapping actor: %s"), *FirstActorName);
	}
}

