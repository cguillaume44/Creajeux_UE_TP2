// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Camera/CameraComponent.h"
// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the camera component
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	CameraComponent->SetupAttachment(GetMesh());
	CameraComponent->bUsePawnControlRotation = true;
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
}

void AMyCharacter::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("release"));
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

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

