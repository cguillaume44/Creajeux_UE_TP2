// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grabable.generated.h"

UCLASS()
class CREAJEUX_UE_TP2_API AGrabable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrabable();

	//create a GUID uproperty called GrabGUID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grab")
	FGuid GrabGUID;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//create the construction script
	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
