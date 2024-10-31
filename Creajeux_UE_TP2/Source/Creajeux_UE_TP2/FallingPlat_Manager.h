// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BillboardComponent.h"
#include "FallingPlat_Manager.generated.h"

UCLASS()
class CREAJEUX_UE_TP2_API AFallingPlat_Manager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFallingPlat_Manager();

	UPROPERTY()
	USceneComponent* Root;

    UPROPERTY(VisibleDefaultsOnly, Category = "Falling Platforms")
    UBillboardComponent* Billboard;

	//Sequence index variable for preview
	UPROPERTY(EditAnywhere, Category = "Falling Platforms", meta = (ClampMin = "0", ClampMax = "2"))
	int32 SequenceIndexToPreview = 0;

	//Editor callable function to refresh the preview
	UFUNCTION(CallInEditor, Category = "Falling Platforms")
	void RefreshPreview();

	void UpdateSequenceIndex(int32 InSequenceIndex);

protected:
	//override the construction script
	virtual void OnConstruction(const FTransform& Transform) override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Falling Platforms")
	int32 RandomSequenceIndex;

};
