// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBlueprintLibrary.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundBase.h"
#include "Particles/ParticleSystem.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"

FVector UMyBlueprintLibrary::GetDirectionBetweenTwoActors(AActor* FromActor, AActor* ToActor)
{
    if (!FromActor || !ToActor) return FVector::ZeroVector;
    return (ToActor->GetActorLocation() - FromActor->GetActorLocation()).GetSafeNormal();
}

void UMyBlueprintLibrary::RotateActorTowardsTarget(AActor* Actor, AActor* Target, float RotationSpeed, float DeltaTime)
{
    if (!Actor || !Target) return;
    FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(Actor->GetActorLocation(), Target->GetActorLocation());
    Actor->SetActorRotation(FMath::RInterpTo(Actor->GetActorRotation(), TargetRotation, DeltaTime, RotationSpeed));
}

void UMyBlueprintLibrary::PlaySoundAtRandomizedPitch(UObject* WorldContextObject, USoundBase* Sound, FVector Location, float MinPitch, float MaxPitch)
{
    if (!Sound) return;
    float Pitch = FMath::RandRange(MinPitch, MaxPitch);
    UGameplayStatics::PlaySoundAtLocation(WorldContextObject->GetWorld(), Sound, Location, 1.0f, Pitch);
}

AActor* UMyBlueprintLibrary::FindClosestActorOfClass(TSubclassOf<AActor> ActorClass, AActor* FromActor)
{
    if (!FromActor) return nullptr;
    UWorld* World = GEngine->GetWorldFromContextObjectChecked(FromActor);
    AActor* ClosestActor = nullptr;
    float MinDistance = FLT_MAX;
    //TActorIterator<AActor> is a template class provided by Unreal Engine 
    // that allows you to iterate over all instances of a specified actor class (ActorClass) 
    // in the world (World). Need #include "EngineUtils.h"
    for (TActorIterator<AActor> It(World, ActorClass); It; ++It)
    {
		if (!*It || *It==FromActor) continue; // Skip if the actor is 'null' or self
        AActor* Actor = *It;
        float Distance = FVector::Dist(FromActor->GetActorLocation(), Actor->GetActorLocation());
        if (Distance < MinDistance)
        {
            MinDistance = Distance;
            ClosestActor = Actor;
        }
    }
    return ClosestActor;
}

float UMyBlueprintLibrary::CalculateDistanceFromActorToPoint(AActor* Actor, FVector Point)
{
    if (!Actor) return 0.0f;
    return FVector::Dist(Actor->GetActorLocation(), Point);
}

void UMyBlueprintLibrary::ApplyRadialForceToObjects(UObject* WorldContextObject, FVector Origin, float Radius, float Strength, bool bImpulse)
{
    if (!WorldContextObject) return;
    UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
    TArray<FHitResult> HitResults;
    FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Radius);

    World->SweepMultiByChannel(HitResults, Origin, Origin, FQuat::Identity, ECC_PhysicsBody, CollisionShape);

    for (FHitResult Hit : HitResults)
    {
        UPrimitiveComponent* PrimComp = Hit.GetComponent();
        if (PrimComp && PrimComp->IsSimulatingPhysics())
        {
            if (bImpulse)
            {
                PrimComp->AddRadialImpulse(Origin, Radius, Strength, ERadialImpulseFalloff::RIF_Linear, true);
            }
            else
            {
                PrimComp->AddRadialForce(Origin, Radius,Strength, ERadialImpulseFalloff::RIF_Linear,true);
            }
        }
    }
}

void UMyBlueprintLibrary::SpawnParticlesAtActorLocation(UParticleSystem* ParticleSystem, AActor* Actor)
{
    if (!ParticleSystem || !Actor) return;
    UGameplayStatics::SpawnEmitterAtLocation(Actor->GetWorld(), ParticleSystem, Actor->GetActorLocation());
}

void UMyBlueprintLibrary::SetObjectMaterialWithIndex(UStaticMeshComponent* MeshComponent, UMaterialInterface* Material, int32 MaterialIndex)
{
    if (!MeshComponent || !Material) return;
    MeshComponent->SetMaterial(MaterialIndex, Material);
}

void UMyBlueprintLibrary::DisableMultipleActors(const TArray<AActor*>& Actors, bool bDisabled)
{
    for (AActor* Actor : Actors)
    {
        if (Actor)
        {
            Actor->SetActorHiddenInGame(!bDisabled);
            Actor->SetActorEnableCollision(bDisabled);
            Actor->SetActorTickEnabled(bDisabled);
        }
    }
}

FVector2D UMyBlueprintLibrary::GetScreenPositionForObject(APlayerController* PlayerController, AActor* Actor)
{
    if (!PlayerController || !Actor) return FVector2D::ZeroVector;
    FVector2D ScreenPosition;
    PlayerController->ProjectWorldLocationToScreen(Actor->GetActorLocation(), ScreenPosition);
    return ScreenPosition;
}

void UMyBlueprintLibrary::ModifyActorScaleOverTime(AActor* Actor, FVector TargetScale, float ScaleSpeed, float DeltaTime)
{
    if (!Actor) return;
    FVector NewScale = FMath::VInterpTo(Actor->GetActorScale(), TargetScale, DeltaTime, ScaleSpeed);
    Actor->SetActorScale3D(NewScale);
}

bool UMyBlueprintLibrary::CheckIfActorIsInLineOfSight(AActor* FromActor, AActor* ToActor, ECollisionChannel TraceChannel, bool bDrawDebug)
{
    if (!FromActor || !ToActor) return false;
    FHitResult HitResult;
    FVector Start = FromActor->GetActorLocation();
    FVector End = ToActor->GetActorLocation();
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(FromActor);
	Params.AddIgnoredActor(ToActor);
    FromActor->GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, TraceChannel, Params);
    if (bDrawDebug)
    {
        DrawDebugLine(FromActor->GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 1.0f);
        DrawDebugPoint(FromActor->GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Green, false, 1.0f);
    }


    return !HitResult.bBlockingHit;
}
