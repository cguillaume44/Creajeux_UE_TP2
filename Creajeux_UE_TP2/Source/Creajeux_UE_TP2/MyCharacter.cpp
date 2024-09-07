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
}

void AMyCharacter::Scroll(float Direction)
{
	UE_LOG(LogTemp, Warning, TEXT("scroll %f"), Direction);
}

void AMyCharacter::StartHoldObj()
{
	UE_LOG(LogTemp, Warning, TEXT("hold"));

	if (HandleCompo && OutHit.GetComponent())
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
	bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit,CameraComponent->GetComponentLocation(),TargetLoc,ECC_Visibility,TraceParams);
	if (bHit)
	{
		//DrawDebugLine(GetWorld(),OutHit.TraceStart,OutHit.Location,	FColor::Green,false, 10.0f, 0,2.0f);

		if (OutHit.GetActor())
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit actor: %s"), *OutHit.GetActor()->GetName());
		}
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

