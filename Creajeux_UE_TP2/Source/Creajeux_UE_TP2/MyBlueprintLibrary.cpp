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
#include "Components/SplineComponent.h"

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

TArray<UActorComponent*> UMyBlueprintLibrary::GetComponentsOfClass(AActor* Actor, TSubclassOf<UActorComponent> ComponentClass)
{
	if (Actor)
	{
		TArray<UActorComponent*> Components;
		Actor->GetComponents(ComponentClass, Components);
		return Components;
	}
	return TArray<UActorComponent*>(); // Return an empty array if the actor is 'null'
}

FVector2D UMyBlueprintLibrary::GetTextureDimensions(UTexture* Texture)
{
	if (Texture)
	{
		return FVector2D(Texture->GetSurfaceWidth(), Texture->GetSurfaceHeight());
	}
	return FVector2D::ZeroVector;
}

TArray<FVector> UMyBlueprintLibrary::ApplyTransformToPoints(const TArray<FVector>& Points, FTransform Transform, bool bWorldSpace)
{
    if (Points.IsValidIndex(0))
    {
	    TArray<FVector> TransformedPoints;
	    for (const FVector Point : Points)
		    {
		    FVector TransformedPoint = bWorldSpace ? Transform.TransformPosition(Point) : Transform.InverseTransformPosition(Point);
		    TransformedPoints.Add(TransformedPoint);
		    }
		return TransformedPoints;
    }
	return TArray<FVector>(); // Return an empty array if the input array is empty
}

TArray<AActor*> UMyBlueprintLibrary::GetActorsOfClassInRadius(UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, FVector Location, float Radius)
{
	if (!WorldContextObject) return TArray<AActor*>();
	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	TArray<AActor*> Actors;
	for (TActorIterator<AActor> It(World, ActorClass); It; ++It)
	{
		if (It && FVector::Dist(Location, It->GetActorLocation()) <= Radius)
		{
			Actors.Add(*It);
		}
	}
	return Actors;
}

int32 UMyBlueprintLibrary::CalculateMemoryFootprint(UObject* Object)
{
	if (Object)
	{
		return Object->GetResourceSizeBytes(EResourceSizeMode::Exclusive);
	}
	return 0;
}

void UMyBlueprintLibrary::DrawDebugBoundingBox(AActor* Actor, FLinearColor Color, float Duration, float Thickness)
{
	if (Actor)
	{
		FVector Origin;
		FVector BoxExtent;
		Actor->GetActorBounds(true, Origin, BoxExtent);
        DrawDebugBox(Actor->GetWorld(), Origin, BoxExtent, Color.ToFColor(true), false, Duration, 0, Thickness);
	}
}

TArray<int32> UMyBlueprintLibrary::GetMeshVertexCount(UStaticMeshComponent* MeshComponent, int32& NumLOD)
{
	if (MeshComponent)
	{
		TArray<int32> VertexCounts;
		UStaticMesh* StaticMesh = MeshComponent->GetStaticMesh();
		if (StaticMesh)
		{
			NumLOD = StaticMesh->GetRenderData()->LODResources.Num();
			for (int32 i = 0; i < NumLOD; i++)
			{
				VertexCounts.Add(StaticMesh->GetRenderData()->LODResources[i].VertexBuffers.PositionVertexBuffer.GetNumVertices());
			}
		}
		return VertexCounts;
	}
	return TArray<int32>(); // Return an empty array if the mesh component is 'null'
}

void UMyBlueprintLibrary::ExportActorsToCSV(UObject* WorldContextObject, FString FileName)
{
	if (!WorldContextObject) return;
	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	FString CSV;
    //Build csv header columns
	CSV += "Name,Class,LocationX,LocationY,LocationZ\n";
	for (TActorIterator<AActor> It(World); It; ++It)
	{
		AActor* Actor = *It;
		FString ActorName = Actor->GetName();
		FString ActorClass = Actor->GetClass()->GetName();
		FString ActorLocX = FString::SanitizeFloat(Actor->GetActorLocation().X);
		FString ActorLocY = FString::SanitizeFloat(Actor->GetActorLocation().Y);
		FString ActorLocZ = FString::SanitizeFloat(Actor->GetActorLocation().Z);
		CSV += ActorName + "," + ActorClass + "," + ActorLocX + "," + ActorLocY + "," + ActorLocZ + "\n";
	}
	FString SaveDirectory = FPaths::ProjectSavedDir();
    if (!FileName.Contains(".csv"))
    {
        FileName = FileName + ".csv";
    }
	FString FinalPath = SaveDirectory + FileName;
	FFileHelper::SaveStringToFile(CSV, *FinalPath);
}

UTexture2D* UMyBlueprintLibrary::CreateProceduralNoiseTexture(int32 Width, int32 Height, float Scale, float NoiseStrength, FLinearColor Color)
{
	UTexture2D* Texture = UTexture2D::CreateTransient(Width, Height);
	Texture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
	Texture->SRGB = 0;
	//adds the object to the root set, which is a collection of objects 
	// that the garbage collector will not automatically clean up.
	//Dangerous for memory leak, use Texture->RemoveFromRoot(); to remove it from the root set in another function?
	Texture->AddToRoot();
	//update the texture resource to reflect the changes made to the texture object
	Texture->UpdateResource();
	FColor* Pixels = static_cast<FColor*>(Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE));
	for (int32 Y = 0; Y < Height; Y++)
	{
		for (int32 X = 0; X < Width; X++)
		{
			float Value = FMath::PerlinNoise2D(FVector2D(X, Y) * Scale) * NoiseStrength;
			Pixels[Y * Width + X] = FLinearColor(Color.R + Value, Color.G + Value, Color.B + Value, Color.A).ToFColor(true);
		}
	}
	Texture->GetPlatformData()->Mips[0].BulkData.Unlock();
	Texture->UpdateResource();
	return Texture;
}

void UMyBlueprintLibrary::TogglePhysicsSimulation(AActor* Actor, bool bSimulate)
{
	if (Actor)
	{
		TArray<UPrimitiveComponent*> PrimitiveComponents;
		Actor->GetComponents<UPrimitiveComponent>(PrimitiveComponents);
		for (UPrimitiveComponent* PrimComp : PrimitiveComponents)
		{
			if (bSimulate)
			{
				PrimComp->SetMobility(EComponentMobility::Movable);
			}
			PrimComp->SetSimulatePhysics(bSimulate);
		}
	}
}

float UMyBlueprintLibrary::CalculateActorMass(AActor* Actor)
{
	if (Actor)
	{
		float TotalMass = 0.0f;
		TArray<UPrimitiveComponent*> PrimitiveComponents;
		Actor->GetComponents<UPrimitiveComponent>(PrimitiveComponents);
		for (UPrimitiveComponent* PrimComp : PrimitiveComponents)
		{
			if (PrimComp->Mobility == EComponentMobility::Movable && PrimComp->IsSimulatingPhysics())
			{
				TotalMass += PrimComp->GetMass();
			}
		}
		return TotalMass;
	}
	return 0.0f;
}