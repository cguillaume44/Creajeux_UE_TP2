// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "Engine/TriggerCapsule.h"
#include "ScalePuzzle.generated.h"

/**
 * 
 */
UCLASS()
class CREAJEUX_UE_TP2_API AScalePuzzle : public ATriggerCapsule
{
	GENERATED_BODY()

public:
	AScalePuzzle();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.1f, UIMax = 500.f))
	float targetWeight=100.f;

	UPROPERTY(VisibleAnywhere)
	float currentWeight = 0.f;

	//DO NOT define this function in the cpp file
	UFUNCTION(BlueprintImplementableEvent)
	void OnWeightChanged(float weight, FColor BarColor);

	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	UFUNCTION()
	void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor);

	FColor SetBarColor(float weight);
	
};
