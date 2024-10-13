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

void AMyCharacter::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("hold"));
	if (CanGrabActor(SightRaycast))
	{
		UPrimitiveComponent* HitComponent = SightRaycast.GetComponent();
		bSavedPhysSim = HitComponent->IsSimulatingPhysics();
		HitComponent->SetPhysicsMaxAngularVelocityInDegrees(100);
		HitComponent->SetSimulatePhysics(true);
		HitComponent->WakeAllRigidBodies();
		HandleCompo->GrabComponentAtLocation(
			HitComponent,
			NAME_None,
			SightRaycast.ImpactPoint);
	}
}

void AMyCharacter::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("release"));
	if (HandleCompo && HandleCompo->GetGrabbedComponent())
	{
		HandleCompo->GetGrabbedComponent()->SetSimulatePhysics(bSavedPhysSim);
		//Reset the default angular velocity
		HandleCompo->GetGrabbedComponent()->SetPhysicsMaxAngularVelocityInDegrees(3600.f);
		HandleCompo->ReleaseComponent();
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

	CamPos = CameraComponent->GetComponentLocation();
	CheckSight();
	UpdtadeHandleLocation();
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyCharacter::UpdtadeHandleLocation()
{
	//Updtade the handle compo target location
	FVector HandleLocation = CamPos + CameraComponent->GetForwardVector() * HoldDistance;
	HandleCompo->SetTargetLocation(HandleLocation);
	//Draw a debub point at the handle location
	DrawDebugPoint(GetWorld(), HandleLocation, 10, FColor::Green, false, 0);
}

void AMyCharacter::CheckSight()
{
	//Line trace by channel
	FCollisionQueryParams TraceParams;
	FVector End = CamPos + CameraComponent->GetForwardVector() * SightLength;
	GetWorld()->LineTraceSingleByChannel(SightRaycast, CamPos, End, ECC_Visibility, TraceParams);
	DrawDebugSphere(GetWorld(),
		SightRaycast.bBlockingHit ? SightRaycast.Location : SightRaycast.TraceEnd,
		10, 10,
		SightRaycast.bBlockingHit ? FColor::Blue : FColor::Red, false, 0);
}

bool AMyCharacter::CanGrabActor(FHitResult Hit)
{
	if (SightRaycast.GetComponent() &&
		SightRaycast.bBlockingHit &&
		SightRaycast.GetComponent()->Mobility == EComponentMobility::Movable)
	{
		return true;
	}

	return false;
}

