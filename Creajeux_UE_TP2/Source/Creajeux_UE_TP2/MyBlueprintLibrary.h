// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class CREAJEUX_UE_TP2_API UMyBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

    // Returns the direction vector from one actor to another.
    UFUNCTION(BlueprintPure, Category = "Custom BP Lib")
    static FVector GetDirectionBetweenTwoActors(AActor* FromActor, AActor* ToActor);

    // Rotates an actor to face a target actor at a specified rotation speed.
    UFUNCTION(BlueprintCallable, Category = "Custom BP Lib")
    static void RotateActorTowardsTarget(AActor* Actor, AActor* Target, float RotationSpeed, float DeltaTime);

    // Plays a sound at a specified location with a randomized pitch within a given range.
    UFUNCTION(BlueprintCallable, Category = "Custom BP Lib")
    static void PlaySoundAtRandomizedPitch(UObject* WorldContextObject, USoundBase* Sound, FVector Location, float MinPitch, float MaxPitch);

    // Finds the closest actor of a specified class to a given actor.
    UFUNCTION(BlueprintPure, Category = "Custom BP Lib")
    static AActor* FindClosestActorOfClass(TSubclassOf<AActor> ActorClass, AActor* FromActor);

    // Calculates the distance from an actor to a specified point.
    UFUNCTION(BlueprintPure, Category = "Custom BP Lib")
    static float CalculateDistanceFromActorToPoint(AActor* Actor, FVector Point);

    // Applies a radial force to objects within a specified radius.
    UFUNCTION(BlueprintCallable, Category = "Custom BP Lib")
    static void ApplyRadialForceToObjects(UObject* WorldContextObject, FVector Origin, float Radius, float Strength, bool bImpulse);

    // Spawns particles at the location of a specified actor.
    UFUNCTION(BlueprintCallable, Category = "Custom BP Lib")
    static void SpawnParticlesAtActorLocation(UParticleSystem* ParticleSystem, AActor* Actor);

    // Sets the material of a static mesh component at a specified index.
    UFUNCTION(BlueprintCallable, Category = "Custom BP Lib")
    static void SetObjectMaterialWithIndex(UStaticMeshComponent* MeshComponent, UMaterialInterface* Material, int32 MaterialIndex);

    // Disables or enables multiple actors.
    UFUNCTION(BlueprintCallable, Category = "Custom BP Lib")
    static void DisableMultipleActors(const TArray<AActor*>& Actors, bool bDisabled);

    // Gets the screen position of an actor for a specified player controller.
    UFUNCTION(BlueprintPure, Category = "Custom BP Lib")
    static FVector2D GetScreenPositionForObject(APlayerController* PlayerController, AActor* Actor);

    // Modifies the scale of an actor over time towards a target scale.
    UFUNCTION(BlueprintCallable, Category = "Custom BP Lib")
    static void ModifyActorScaleOverTime(AActor* Actor, FVector TargetScale, float ScaleSpeed, float DeltaTime);

    // Checks if one actor is in the line of sight of another actor using a specified trace channel.
    UFUNCTION(BlueprintPure, Category = "Custom BP Lib")
    static bool CheckIfActorIsInLineOfSight(AActor* FromActor, AActor* ToActor, ECollisionChannel TraceChannel = ECC_Visibility, bool bDrawDebug=false);

    //Retrieves all components of a specified class attached to an actor.
	UFUNCTION(BlueprintPure, Category = "Custom BP Lib")
	static TArray<UActorComponent*> GetComponentsOfClass(AActor* Actor, TSubclassOf<UActorComponent> ComponentClass);

    //Retrieves the width and height of a texture asset.
	UFUNCTION(BlueprintPure, Category = "Custom BP Lib")
	static FVector2D GetTextureDimensions(UTexture* Texture);

    //Applies a transform to an array of FVector points, returning the new positions in world or local space.
	UFUNCTION(BlueprintPure, Category = "Custom BP Lib")
	static TArray<FVector> ApplyTransformToPoints(const TArray<FVector>& Points, FTransform Transform, bool bWorldSpace = true);

    //Retrieves all actors of a specified class within a certain radius of a location.
	UFUNCTION(BlueprintPure, Category = "Custom BP Lib")
	static TArray<AActor*> GetActorsOfClassInRadius(UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, FVector Location, float Radius);

    //Calculates the memory footprint of a given UObject and its components.
	UFUNCTION(BlueprintPure, Category = "Custom BP Lib")
    static int32 CalculateMemoryFootprint(UObject* Object);

    //Draws a debug bounding box around an actor or component.
	UFUNCTION(BlueprintCallable, Category = "Custom BP Lib")
	static void DrawDebugBoundingBox(AActor* Actor, FLinearColor Color = FLinearColor::Red, float Duration = 0.0f, float Thickness = 1.0f);

    //Calculates the total number of vertices in a static mesh or skeletal mesh.
	UFUNCTION(BlueprintPure, Category = "Custom BP Lib")
	static TArray<int32> GetMeshVertexCount(UStaticMeshComponent* MeshComponent, int32& NumLOD);

    //Exports the names, locations, and classes of all actors in the level to a CSV file.
	UFUNCTION(BlueprintCallable, Category = "Custom BP Lib")
	static void ExportActorsToCSV(UObject* WorldContextObject, FString FileName);

    //Creates a procedural Perlin noise texture at runtime.
	UFUNCTION(BlueprintCallable, Category = "Custom BP Lib")
	static UTexture2D* CreateProceduralNoiseTexture(int32 Width, int32 Height, float Scale, float NoiseStrength, FLinearColor Color);

    //Toggles the physics simulation state for all physics-enabled components of an actor.
	UFUNCTION(BlueprintCallable, Category = "Custom BP Lib")
	static void TogglePhysicsSimulation(AActor* Actor, bool bSimulate);

    // Calculates the combined mass of all physics-enabled components in an actor.
	UFUNCTION(BlueprintPure, Category = "Custom BP Lib")
	static float CalculateActorMass(AActor* Actor);
};
