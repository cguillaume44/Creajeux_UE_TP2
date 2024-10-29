// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "KeyTrigger.generated.h"

/**
 * 
 */
UCLASS()
class CREAJEUX_UE_TP2_API AKeyTrigger : public ATriggerBox
{
	GENERATED_BODY()

public:
	// Constructor
	AKeyTrigger();

	int32 KeyCount = 0;

protected:
	// Static mesh components
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComponent1;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComponent2;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComponent3;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComponent4;

	//Pointer to a door actor object in the level
	UPROPERTY(EditAnywhere, Category = "Default")
	class ADoor* DoorActor;

	//Pointer to a the GM
	UPROPERTY(EditAnywhere, Category = "Default")
	class ATempleRaiderGM* GM;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Function to handle the begin overlap event
	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	void AttachKeyAndCount(AActor* OtherActor);
};
