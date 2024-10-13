// Fill out your copyright notice in the Description page of Project Settings.


#include "CageTrigger.h"

ACageTrigger::ACageTrigger()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACageTrigger::BeginPlay()
{
	Super::BeginPlay();
	// Bind the overlap events
	OnActorBeginOverlap.AddDynamic(this, &ACageTrigger::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ACageTrigger::OnOverlapEnd);

	if (MovingActorRef)
	{
		StartingLocation = MovingActorRef->GetActorLocation();
	}
}

void ACageTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsInterpolating && MovingActorRef)
	{
		FVector CurrentLocation = MovingActorRef->GetActorLocation();
		FVector NewLocation = FMath::VInterpTo(CurrentLocation, InterpTargetLocation, DeltaTime, InterpSpeed);
		MovingActorRef->SetActorLocation(NewLocation);

		if (FVector::Dist(NewLocation, InterpTargetLocation) < UE_KINDA_SMALL_NUMBER)
		{
			bIsInterpolating = false;
		}
	}
}
void ACageTrigger::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && (OtherActor != this) && OtherActor->ActorHasTag("Grabbable"))
	{
		if (MovingActorRef)
		{
			InterpTargetLocation = StartingLocation;
			bIsInterpolating = true;
		}
	}
}

void ACageTrigger::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && (OtherActor != this) && OtherActor->ActorHasTag("Grabbable"))
	{
		//check if GetOverlappingActors returns 0
		TArray<AActor*> OverlappingActors;
		GetOverlappingActors(OverlappingActors);

		if (MovingActorRef && !IsOverlappingActor(this) && OverlappingActors.Num()==0)
		{
			InterpTargetLocation = TargetLocation;
			bIsInterpolating = true;
		}
	}
}