// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Sound/SoundCue.h"
#include "SoundManager.generated.h"

//Ideally , this class should be a singleton, but for the sake of simplicity, we will keep it as a component
//Another way to handle this is to put it in the GameMode class directly 
//Another handy way to handle this is to use the GameInstance class or a subsytem.
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CREAJEUX_UE_TP2_API USoundManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USoundManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Sound cue property
	UPROPERTY(EditAnywhere, Category = "Sound lib")
	USoundCue* SimonCue;

	//Sound cue property
	UPROPERTY(EditAnywhere, Category = "Sound lib")
	USoundCue* AmbientCue;

	//Sound cue property
	UPROPERTY(EditAnywhere, Category = "Sound lib")
	USoundCue* WinCue;
	
	//Volume property SFX
	UPROPERTY(EditAnywhere, Category = "Sound volume", meta = (ClampMin = 0.1f, UIMax = 1.f))
	float volumeSFX = 1.0f;

	//Volume property SFX
	UPROPERTY(EditAnywhere, Category = "Sound volume", meta = (ClampMin = 0.1f, UIMax = 1.f))
	float volumeAmbient = 1.0f;

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayAmbientSound();

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayWinSound();

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlaySimonSound(float pitch, FVector Location);

};
