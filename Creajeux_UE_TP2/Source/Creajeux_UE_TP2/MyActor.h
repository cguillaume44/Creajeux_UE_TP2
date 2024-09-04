// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractInterface.h"
#include "MyActor.generated.h"

UCLASS()
class CREAJEUX_UE_TP2_API AMyActor : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// BP Function override, Use name+suffix "_Implementation" 
	void InteractBP_Implementation() override;

	//CPP only function implementation
	virtual void InteractCPP() override;


};
