// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "SoundManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create the root component
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	//create the static mesh components
	DoorMesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh1"));
	DoorMesh1->SetupAttachment(Root);
    
	DoorMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh2"));
	DoorMesh2->SetupAttachment(Root);


}

void ADoor::SkipOpendoor()
{
	if (!bDoorOpen)
	{
		bDoorOpen = true;
		TimeElapsed = RotationDuration;
		RotateDoors(0.0f);
	}
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
    AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
    if (GameMode)
    {
        SoundManager = GameMode->GetComponentByClass<USoundManager>();
    }
}

void ADoor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (!DoorGUID.IsValid())
	{
		DoorGUID = FGuid::NewGuid();
	}
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoor::OnDoorTrigger()
{
    TimeElapsed = 0.0f;
    GetWorld()->GetTimerManager().SetTimer(RotationTimerHandle, [this]()
        {
            float DeltaTime = GetWorld()->GetDeltaSeconds();
            TimeElapsed += DeltaTime;

            RotateDoors(DeltaTime);

            if (TimeElapsed >= RotationDuration)
            {
                GetWorld()->GetTimerManager().ClearTimer(RotationTimerHandle);
            }
        }, 0.01f, true);
}

void ADoor::CheckOpenDoor()
{
    if (!bDoorOpen)
	{
		bDoorOpen = true;
        OnDoorTrigger();
        SoundManager->PlayWinSound();
	}
}

void ADoor::RotateDoors(float DeltaTime)
{
    FRotator InitialRotation1 = FRotator::ZeroRotator; // Initial rotation
    FRotator TargetRotation1 = FRotator(0.f, 90.f, 0.f); // Example target rotation

    FRotator InitialRotation2 = FRotator::ZeroRotator; // Initial rotation
    FRotator TargetRotation2 = FRotator(0.f, -90.f, 0.f); // Example target rotation

    float Alpha = FMath::Clamp(TimeElapsed / RotationDuration, 0.0f, 1.0f);
    FRotator NewRotation1 = FMath::Lerp(InitialRotation1, TargetRotation1, Alpha);
    FRotator NewRotation2 = FMath::Lerp(InitialRotation2, TargetRotation2, Alpha);

    DoorMesh1->SetRelativeRotation(NewRotation1);
    DoorMesh2->SetRelativeRotation(NewRotation2);
}

