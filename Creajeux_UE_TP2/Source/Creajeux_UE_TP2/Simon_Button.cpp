// Fill out your copyright notice in the Description page of Project Settings.


#include "Simon_Button.h"
#include "Simon_Manager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "SoundManager.h"

// Sets default values
ASimon_Button::ASimon_Button()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the root component
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	// Create the button mesh component
	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	ButtonMesh->SetupAttachment(RootComponent);

	// Create the ring mesh component
	RingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RingMesh"));
	RingMesh->SetupAttachment(RootComponent);
}

void ASimon_Button::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// Create a dynamic material instance for the button mesh
	DynMaterial = ButtonMesh->CreateAndSetMaterialInstanceDynamic(0);
	if (DynMaterial)
	{
		// Set the color of the dynamic material instance
		DynMaterial->SetVectorParameterValue("Color_Tint_R", ButtonColor);
		DynMaterial->SetScalarParameterValue("EmissiveStrength_Final", 0.f);
	}
}


// Called when the game starts or when spawned
void ASimon_Button::BeginPlay()
{
	Super::BeginPlay();
	DynMaterial = Cast<UMaterialInstanceDynamic>(ButtonMesh->GetMaterial(0));

	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
	if (GameMode)
	{
		SoundManager = GameMode->GetComponentByClass<USoundManager>();
	}

}

// Called every frame
void ASimon_Button::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASimon_Button::PressButton_Implementation()
{
	//call button pressed on manager if valid
	if (Manager)
	{
		Manager->ButtonPressed(this);
		PressAnim();
	}


}

void ASimon_Button::LightUp()
{
	//This is called when the button is pressed AND when the sequence is played so it's better to put it here.
	//BUT it's not the best system.
	Sound();
	
	if (DynMaterial)
	{
		DynMaterial->SetScalarParameterValue("EmissiveStrength_Final", 1.f);
	}
}

void ASimon_Button::LightDown()
{

	if (DynMaterial)
	{
		DynMaterial->SetScalarParameterValue("EmissiveStrength_Final", 0.f);
	}
}

void ASimon_Button::PressAnim()
{
	//call light up and 0.5 secs later call light down
	LightUp();
	FTimerHandle PressTimer;
	GetWorldTimerManager().SetTimer(PressTimer, this, &ASimon_Button::LightDown, 0.5f, false);
}

void ASimon_Button::Sound()
{
	if (bPlaySound && SoundManager)	SoundManager->PlaySimonSound(SoundPitch, GetActorLocation());
}




