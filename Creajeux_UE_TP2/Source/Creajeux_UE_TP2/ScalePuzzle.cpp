// Fill out your copyright notice in the Description page of Project Settings.


#include "ScalePuzzle.h"
#include "Door.h"

AScalePuzzle::AScalePuzzle()
{
	//bind the functions to the triggers
	OnActorBeginOverlap.AddDynamic(this, &AScalePuzzle::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &AScalePuzzle::OnOverlapEnd);
}

void AScalePuzzle::BeginPlay()
{
	Super::BeginPlay();

}

void AScalePuzzle::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor)
	{
		// Get the root component and cast it to UPrimitiveComponent
		UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
		if (RootComp && RootComp->IsSimulatingPhysics())
		{
			// Add the weight of the overlapped actor to the current weight
			currentWeight += RootComp->GetMass();

			// Remap the current weight to the target weight between 0 and 1, can be greater than 1
			float TempWeight = currentWeight / targetWeight;
			OnWeightChanged(TempWeight, SetBarColor(TempWeight));
			if (DoorActor && currentWeight == targetWeight)
			{
				DoorActor->CheckOpenDoor();
			}
		}
	}
}

void AScalePuzzle::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	
	if (OtherActor)
	{
		// Get the root component and cast it to UPrimitiveComponent
		UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
		if (RootComp && RootComp->IsSimulatingPhysics())
		{
			// Remove the weight of the overlapped actor from the current weight
			currentWeight -= RootComp->GetMass();

			// Remap the current weight to the target weight between 0 and 1, can be greater than 1
			float TempWeight = FMath::Clamp(currentWeight / targetWeight, 0.f, 1.f);
			OnWeightChanged(TempWeight, SetBarColor(TempWeight));
		}
	}
}

FColor AScalePuzzle::SetBarColor(float weight)
{
	if (weight < 1)
	{
		return FColor::Blue;
	}
	else if (weight == 1)
	{
		return FColor::Green;
	}
	else
	{
		return FColor::Red;
	}
}
