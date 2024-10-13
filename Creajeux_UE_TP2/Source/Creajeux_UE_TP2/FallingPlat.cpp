// Fill out your copyright notice in the Description page of Project Settings.


#include "FallingPlat.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"

// Sets default values
AFallingPlat::AFallingPlat()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	PlatMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plateform Mesh"));
	PlatMesh->SetupAttachment(RootComponent);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetupAttachment(PlatMesh);

}

// Called when the game starts or when spawned
void AFallingPlat::BeginPlay()
{
	Super::BeginPlay();

	//bind on begin overlap on the collision box to a new function
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AFallingPlat::OnTriggerOverlap);
	
}

// Called every frame
void AFallingPlat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AFallingPlat::OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//destroy the actor if the other actor is the player character and the bool status is true
	if (FindStatus(SequenceIndex) && OtherActor->IsA(ACharacter::StaticClass()))
	{
		//Wait for 1 second before destroying the actor
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AFallingPlat::DelayedDestroyActor, 0.1f, false);
		
	}
}

bool AFallingPlat::FindStatus(int32 InSequenceIndex)
{
	//switch on the sequence index and return the corresponding status
	switch (SequenceIndex)
	{
	case 0:
		return bIsFakeSeq0;
		break;
	case 1:
		return bIsFakeSeq1;
		break;
	case 2:
		return bIsFakeSeq2;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Sequence not found"));
		return false;
		break;
	}
}

void AFallingPlat::DelayedDestroyActor()
{
	Destroy();
}

