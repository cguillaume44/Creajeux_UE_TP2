// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ComplexHierarchyActor.generated.h"

UCLASS()
class CREAJEUX_UE_TP2_API AComplexHierarchyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AComplexHierarchyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
    // Root component for organizing the hierarchy
    UPROPERTY(VisibleAnywhere, Category = "Components")
    USceneComponent* RootSceneComponent;

    // Static mesh for visual representation
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* MainMesh;

    // Sphere component attached to the mesh
    UPROPERTY(VisibleAnywhere, Category = "Components")
    class USphereComponent* SphereCollider;

    // Spot light attached to the sphere
    UPROPERTY(VisibleAnywhere, Category = "Components")
    class USpotLightComponent* SpotLight;

    // Arrow component to indicate direction
    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UArrowComponent* DirectionIndicator;

    // Box Collider attached to the Main Mesh
    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UBoxComponent* BoxCollider;

    // Point light attached to the Box Collider
    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UPointLightComponent* PointLight;

    // Audio component attached to the Sphere Collider
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UAudioComponent* AudioComponent;

    // Camera attached to the Root Scene Component
    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UCameraComponent* CameraComponent;

};
