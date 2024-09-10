// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Camera/CameraComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the camera component
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	CameraComponent->SetupAttachment(GetMesh());
	CameraComponent->bUsePawnControlRotation = true;

	//Create the physic handle compo
	HandleCompo = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("HandleCompo"));
}

void AMyCharacter::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("interact"));
	if (ActorInSight)
	{
		IInteractInterface::Execute_PressButton(ActorInSight);
	}
}

void AMyCharacter::Scroll(float Direction)
{
	UE_LOG(LogTemp, Warning, TEXT("scroll %f"), Direction);
}

void AMyCharacter::StartHoldObj()
{
	UE_LOG(LogTemp, Warning, TEXT("hold"));

	if (HandleCompo && OutHit.GetComponent() && OutHit.GetActor()->ActorHasTag("Grabbable"))
	{
		//To use the UPhysicsHandleComponent the grabbed component must simulate physic
		OutHit.GetComponent()->SetSimulatePhysics(true);
		HandleCompo->GrabComponent(OutHit.GetComponent(), NAME_None, TargetLoc,true);
	}
}

void AMyCharacter::ReleaseObj()
{
	UE_LOG(LogTemp, Warning, TEXT("release"));

	if (HandleCompo && HandleCompo->GetGrabbedComponent())
	{
		HandleCompo->GetGrabbedComponent()->SetSimulatePhysics(false);
		HandleCompo->ReleaseComponent();
	}
}

void AMyCharacter::CheckSight()
{

	//Line trace by channel
	FCollisionQueryParams TraceParams;
	bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, CameraComponent->GetComponentLocation(), TargetLoc, ECC_Visibility, TraceParams);

	//Check if we have a valid actor hit that implements the interface
	//In C++, the logical AND operator (&&) evaluates expressions from left to right and uses short-circuit evaluation. 
	//This means that if the first condition is false, the second condition is not evaluated.
	//Here we use getclass>ImplementsInterface to ensure the BP actor has the class, if you try to cast it won't work.
	if (bHit && OutHit.GetActor() && IsImplementingInteractInterface(OutHit.GetActor()))
	{
		if (OutHit.GetActor() != ActorInSight)
		{
			ResetActorInSight();
			ActorInSight = OutHit.GetActor();
			IInteractInterface::Execute_LookAt(ActorInSight);
		}
		//Do nothing if the actor is the same
	}
	else
	{
		//If we don't hit anything or an actor without interface, we reset the actor in sight
		ResetActorInSight();
	}

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HandleCompo && CameraComponent)
	{
		//The target loc is used by the line trace and by the HandleCompo.
		TargetLoc = (CameraComponent->GetForwardVector() * SightLenght) + CameraComponent->GetComponentLocation();
		//We check sight in tick because the line of sight is always changing.
		//Later we will send "lookat" & "StopLookAt" to the actors so we need it in tick.
		// This can be improved.
		//In another gamedesign we could check the sight only on click.
		CheckSight();
		//If you use the line trace hit as a target loc, the object will hit on the first frame, then it will get it closer every frame.
		HandleCompo->SetTargetLocation(TargetLoc);
	}
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMyCharacter::ResetActorInSight()
{
	if (ActorInSight && IsImplementingInteractInterface(ActorInSight))
	{
			IInteractInterface::Execute_StopLookAt(ActorInSight);
	}
	//Unset the actor in sight, used in CheckSight() and in interact()
	ActorInSight = nullptr;
}

bool AMyCharacter::IsImplementingInteractInterface(AActor* Actor)
{
	if (Actor && Actor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
	{
		return true;
	}
	return false;
}

