// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractInterface.h"
#include "Simon_Button.generated.h"

UCLASS()
class CREAJEUX_UE_TP2_API ASimon_Button : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASimon_Button();

	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent* ButtonMesh;

	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent* RingMesh;


protected:
	//overide the construction script
	virtual void OnConstruction(const FTransform& Transform) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//create a color variable to store the color of the button
	UPROPERTY(EditAnywhere, Category = "Simon Button")
	FLinearColor ButtonColor;

	//implement the pressbutton function from the interface
	virtual void PressButton_Implementation() override;

	//create a pointer to the manager
	class ASimon_Manager* Manager;

	UMaterialInstanceDynamic* DynMaterial;

	void LightUp();

	void LightDown();

	void PressAnim();

};
