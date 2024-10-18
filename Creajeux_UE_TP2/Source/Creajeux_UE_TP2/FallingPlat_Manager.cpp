// Fill out your copyright notice in the Description page of Project Settings.


#include "FallingPlat_Manager.h"
#include "Components/BillboardComponent.h"
#include "FallingPlat.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFallingPlat_Manager::AFallingPlat_Manager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetupAttachment(RootComponent);

}

void AFallingPlat_Manager::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	RefreshPreview();
}

// Called when the game starts or when spawned
void AFallingPlat_Manager::BeginPlay()
{
	Super::BeginPlay();

	//set a random sequence index between 0 and 2, must be updated depending on GD
	RandomSequenceIndex = FMath::RandRange(0, 2);

	//More elegant ways can be used for this, but for the sake of simplicity we will 
	//loop through all the actors of the FallingPlat class and set the sequence index
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFallingPlat::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		AFallingPlat* FallingPlat = Cast<AFallingPlat>(Actor);
		if (FallingPlat)
		{
			FallingPlat->SequenceIndex = RandomSequenceIndex;
		}
	}
	
}

// Called every frame
void AFallingPlat_Manager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFallingPlat_Manager::RefreshPreview()
{
	//flush the persistent debug lines
	FlushPersistentDebugLines(GetWorld());

	//Get all the actors of the FallingPlat class
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFallingPlat::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		AFallingPlat* FallingPlat = Cast<AFallingPlat>(Actor);
		if (FallingPlat)
		{
			bool bIsFake = false;
			switch (SequenceIndexToPreview)
			{
			case 0: bIsFake = FallingPlat->bIsFakeSeq0; break;
			case 1: bIsFake = FallingPlat->bIsFakeSeq1; break;
			case 2: bIsFake = FallingPlat->bIsFakeSeq2; break;
			}
			//define the debug sphere color depending on the platform being fake or not
			FColor SphereColor = bIsFake ? FColor::Red : FColor::Green;
			FVector SphereLocation = FallingPlat->GetActorLocation() + FVector(0.0f, 0.0f, 100.0f);

			DrawDebugSphere(GetWorld(), SphereLocation, 50.0f, 12, SphereColor, true, 0.0f, 0, 1.0f);
		}
	}
}