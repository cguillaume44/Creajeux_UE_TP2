// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabable.h"

// Sets default values
AGrabable::AGrabable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGrabable::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGrabable::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (!GrabGUID.IsValid())
	{
		GrabGUID = FGuid::NewGuid();
	}
}

// Called every frame
void AGrabable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

