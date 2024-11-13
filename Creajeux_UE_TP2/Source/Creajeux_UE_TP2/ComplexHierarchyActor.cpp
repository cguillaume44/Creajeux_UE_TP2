// Fill out your copyright notice in the Description page of Project Settings.


#include "ComplexHierarchyActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/SphereComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/AudioComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AComplexHierarchyActor::AComplexHierarchyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Root Component
    RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
    RootComponent = RootSceneComponent;

    // Main Mesh attached to Root
    MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
    MainMesh->SetupAttachment(RootComponent);

    // Sphere Collider attached to Main Mesh
    SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
    SphereCollider->SetupAttachment(MainMesh);

    // Spot Light attached to Sphere Collider
    SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
    SpotLight->SetupAttachment(SphereCollider);

    // Arrow Component attached to Root for direction indication
    DirectionIndicator = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
    DirectionIndicator->SetupAttachment(RootComponent);

    // Box Collider attached to Main Mesh
    BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
    BoxCollider->SetupAttachment(MainMesh);

    // Point Light attached to Box Collider
    PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
    PointLight->SetupAttachment(BoxCollider);

    // Audio Component attached to Sphere Collider
    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
    AudioComponent->SetupAttachment(SphereCollider);

    // Camera Component attached to Root
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(RootComponent);

    // Configure Sphere Collider
    SphereCollider->InitSphereRadius(50.0f);
    SphereCollider->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

    // Configure Spot Light
    SpotLight->Intensity = 3000.0f;

    // Configure Box Collider
    BoxCollider->SetBoxExtent(FVector(100.0f, 100.0f, 50.0f));

    // Configure Point Light
    PointLight->Intensity = 2000.0f;
    PointLight->SetLightColor(FColor::Red);

    // Configure Audio Component
    AudioComponent->bAutoActivate = false; // Doesn't play until triggered

    // Configure Camera
    CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 50.0f)); // Offset for better view angle

}

// Called when the game starts or when spawned
void AComplexHierarchyActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AComplexHierarchyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

