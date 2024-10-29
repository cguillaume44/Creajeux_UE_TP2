// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FallingPlat.generated.h"

UCLASS()
class CREAJEUX_UE_TP2_API AFallingPlat : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFallingPlat();

	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent* PlatMesh;

	UPROPERTY(VisibleDefaultsOnly)
	class UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere)
	bool bIsFakeSeq0 = false;

	UPROPERTY(EditAnywhere)
	bool bIsFakeSeq1 = false;

	UPROPERTY(EditAnywhere)
	bool bIsFakeSeq2 = false;

	//create a GUID uproperty called GrabGUID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grab")
	FGuid PlatGUID;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//call on construction script
	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnTriggerOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	int32 SequenceIndex=0;

	//Return the status depending on the sequence index
	bool FindStatus(int32 InSequenceIndex);

	void DelayedDestroyActor();
};
