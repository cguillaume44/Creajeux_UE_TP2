// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
USoundManager::USoundManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USoundManager::BeginPlay()
{
	Super::BeginPlay();
	PlayAmbientSound();
	// ...
	
}


// Called every frame
void USoundManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USoundManager::PlayAmbientSound()
{
	//play sound cue
	if (AmbientCue)
	{
		UGameplayStatics::PlaySound2D(this, AmbientCue,volumeAmbient);
	}
}

void USoundManager::PlayWinSound()
{
	//play sound cue
	if (WinCue)
	{
		UGameplayStatics::PlaySound2D(this, WinCue,volumeSFX);
	}
}

void USoundManager::PlaySimonSound(float pitch, FVector Location)
{
	//play sound cue
	if (SimonCue)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SimonCue, Location, volumeSFX, pitch);
	}
}

