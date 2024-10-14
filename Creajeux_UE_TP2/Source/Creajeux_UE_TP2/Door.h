// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class CREAJEUX_UE_TP2_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

	UPROPERTY(EditAnywhere)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* DoorMesh1;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* DoorMesh2;

	UPROPERTY(EditAnywhere)
	float RotationDuration = 3.0f;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle RotationTimerHandle;

	bool bDoorOpen = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//implement the OpenDoor function from the interface
	void CheckOpenDoor();

private:

	UFUNCTION(CallInEditor)
	void OnDoorTrigger();

	float TimeElapsed = 0.0f;

	void RotateDoors(float DeltaTime);
};
