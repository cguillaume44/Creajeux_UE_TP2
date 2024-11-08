// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Camera/CameraComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "DrawDebugHelpers.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"


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

	//Create the Niagara component
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(RootComponent);
}

void AMyCharacter::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("interact"));
	if (ActorInSight && IsImplementingInteractInterface(ActorInSight))
	{
		IInteractInterface::Execute_PressButton(ActorInSight);
	}
}

void AMyCharacter::Scroll(float Direction)
{
	UE_LOG(LogTemp, Warning, TEXT("scroll %f"), Direction);

	// Update & Clamp HoldDistance between 50 and 500
	HoldDistance = FMath::Clamp(HoldDistance + Direction * 10.0f, 50.0f, 500.0f);

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
		HandleCompo->GetGrabbedComponent()->GetOwner()->Tags.Add("Held");
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
		HandleCompo->GetGrabbedComponent()->GetOwner()->Tags.Remove("Held");
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
	UpdateNiagaraCursor();
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
	//DrawDebugPoint(GetWorld(), HandleLocation, 10, FColor::Green, false, 0);
}

void AMyCharacter::CheckSight()
{
	//Line trace by channel
	FCollisionQueryParams TraceParams;
	FVector End = CamPos + CameraComponent->GetForwardVector() * SightLength;
	GetWorld()->LineTraceSingleByChannel(SightRaycast, CamPos, End, ECC_Visibility, TraceParams);
	//DrawDebugSphere(GetWorld(),
	//	SightRaycast.bBlockingHit ? SightRaycast.Location : SightRaycast.TraceEnd,
	//	10, 10,
	//	SightRaycast.bBlockingHit ? FColor::Blue : FColor::Red, false, 0);

	if (SightRaycast.bBlockingHit && SightRaycast.GetActor() && IsImplementingInteractInterface(SightRaycast.GetActor()))
	{
		if (SightRaycast.GetActor() != ActorInSight && !SightRaycast.GetActor()->ActorHasTag("Held"))
		{
			ResetActorInSight();
			ActorInSight = SightRaycast.GetActor();
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

bool AMyCharacter::CanGrabActor(FHitResult Hit)
{
	return SightRaycast.GetComponent() &&
		SightRaycast.bBlockingHit &&
		IsImplementingInteractInterface(Hit.GetActor()) &&
		SightRaycast.GetActor()->ActorHasTag("Grabbable");
}

void AMyCharacter::UpdateNiagaraCursor()
{
	if (NiagaraComponent && NiagaraComponent->GetAsset())
	{
		FVector NiagaraLocation;
		FLinearColor NiagColor;

		if (HandleCompo->GetGrabbedComponent())
		{
			NiagaraLocation = HandleCompo->GetGrabbedComponent()->GetComponentLocation();
			NiagColor = NiagColorHeld;
		}
		else
		{
			NiagaraLocation = SightRaycast.bBlockingHit ? SightRaycast.Location : SightRaycast.TraceEnd;
			NiagColor = SightRaycast.bBlockingHit ? NiagColorColli : NiagColorDefault;
		}

		NiagaraComponent->SetNiagaraVariableLinearColor(FString("CursorColor"), NiagColor);
		NiagaraComponent->SetNiagaraVariablePosition(FString("Position"), NiagaraLocation);
	}
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
	if (Actor)
	{
		return Actor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass());
	}
	return false;
}