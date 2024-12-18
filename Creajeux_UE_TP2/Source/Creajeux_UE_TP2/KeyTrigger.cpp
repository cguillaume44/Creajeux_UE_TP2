// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyTrigger.h"
#include "Door.h"
#include "MyCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "TempleRaiderGM.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Constructor
AKeyTrigger::AKeyTrigger()
{
	// Create the static mesh components
	//If we need a stronger system, we could create as many components as a key count property.
	MeshComponent1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent1"));
	MeshComponent1->SetupAttachment(RootComponent);

	MeshComponent2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent2"));
	MeshComponent2->SetupAttachment(RootComponent);

	MeshComponent3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent3"));
	MeshComponent3->SetupAttachment(RootComponent);

	MeshComponent4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent4"));
	MeshComponent4->SetupAttachment(RootComponent);

	SetActorHiddenInGame(false);
}


void AKeyTrigger::BeginPlay()
{
	Super::BeginPlay();

	// Bind the OnOverlapBegin function to the BeginOverlap event
	OnActorBeginOverlap.AddDynamic(this, &AKeyTrigger::OnOverlapBegin);

	//get the GM and save it
	GM = Cast<ATempleRaiderGM>(UGameplayStatics::GetGameMode(GetWorld()));
}

void AKeyTrigger::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	// Check if the overlapping actor is valid and not the same as this actor
	if (OtherActor && OtherActor != this && OtherActor->ActorHasTag("Key"))
	{
		AttachKeyAndCount(OtherActor);
		GM->TreasureIndex++;
		// Implement your logic here, for example, interacting with the DoorActor
		if (DoorActor && KeyCount>=4)
		{
			// Call a function on the DoorActor, e.g., OpenDoor()
			DoorActor->CheckOpenDoor();
			TArray<AActor*> Keys;
			UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Key", Keys);
			for (AActor* Key : Keys)
			{
				if (Key)
				{
					Key->Destroy();
				}
			}
			//Spawn the Niagara system at the mesh compo location
			PlayNiagaraSystem(MeshComponent1);
			PlayNiagaraSystem(MeshComponent2);
			PlayNiagaraSystem(MeshComponent3);
			PlayNiagaraSystem(MeshComponent4);

			//Destroy the key trigger
			Destroy();
		}
	}
}

void AKeyTrigger::AttachKeyAndCount(AActor* OtherActor)
{
	if (OtherActor)
	{
		//Swith on the key count, depending on the index attach the other actor to the corresponding mesh component
		USceneComponent* ToAttach;
		switch (KeyCount)
		{
		case 0:
			ToAttach = MeshComponent1;
			break;
		case 1:
			ToAttach = MeshComponent2;
			break;
		case 2:
			ToAttach = MeshComponent3;
			break;
		case 3:
			ToAttach = MeshComponent4;
			break;
		default:
			ToAttach = MeshComponent1;
			break;
		}
		APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
		// Cast the player pawn to AMyCharacter and call Release
		// could be done differently with a interface or something else 
		// but it's the end of the project I'm tired.
		if (PlayerPawn)
		{
			AMyCharacter* MyCharacter = Cast<AMyCharacter>(PlayerPawn);
			if (MyCharacter)
			{
				MyCharacter->Release();
			}
		}
		OtherActor->AttachToComponent(ToAttach, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		//get the key trigger world rotation and set it to the key
		OtherActor->SetActorRotation(FRotator(0,90,0));
		OtherActor->Tags.Remove("Grabbable");
		KeyCount++;
	}
}

void AKeyTrigger::PlayNiagaraSystem(USceneComponent* Compo)
{
	if (NiagaraSystem && Compo)
	{
		//Spawn the Niagara system at the given location
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraSystem, Compo->GetComponentLocation(), Compo->GetComponentRotation(), Compo->GetComponentScale());
	}
}