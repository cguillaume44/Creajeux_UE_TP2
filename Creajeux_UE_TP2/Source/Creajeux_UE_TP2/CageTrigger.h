// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "Engine/StaticMeshActor.h"
#include "CageTrigger.generated.h"


/**
 * 
 */
UCLASS()
class CREAJEUX_UE_TP2_API ACageTrigger : public ATriggerBox
{
	GENERATED_BODY()

public:

	ACageTrigger();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//Will be set automatically in begin play
	FVector StartingLocation;

	//the location where the actor will block the path
	UPROPERTY(EditAnywhere)
	FVector TargetLocation;

	//the actor that will move
	UPROPERTY(EditAnywhere)
	AStaticMeshActor* MovingActorRef;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.1f, ClampMax = 5.f))
	float InterpSpeed = 1.0f;

private:
	bool bIsInterpolating=false;
	FVector InterpTargetLocation;

	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);
};