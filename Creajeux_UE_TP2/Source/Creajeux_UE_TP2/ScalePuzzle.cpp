// Fill out your copyright notice in the Description page of Project Settings.


#include "ScalePuzzle.h"

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
		//add the weight of the overlapped actor to the current weight
		currentWeight += OtherActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		
		//remap the current weight to the target weight between 0 and 1 can be greater than 1
		//the remap is used in the setpercent of the progress bar
		float TempWeight = currentWeight / targetWeight;
		OnWeightChanged(TempWeight, SetBarColor(TempWeight));
	}
}

void AScalePuzzle::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	
	if (OtherActor)
	{
		//remove the weight of the overlapped actor to the current weight
		currentWeight -= OtherActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();

		//remap the current weight to the target weight between 0 and 1 can be greater than 1
		//the remap is used in the setpercent of the progress bar
		float TempWeight = currentWeight / targetWeight;
		OnWeightChanged(TempWeight, SetBarColor(TempWeight));
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
