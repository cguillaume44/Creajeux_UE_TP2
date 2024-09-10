// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InteractInterface.h"
#include "MyCharacter.generated.h"

UCLASS()
class CREAJEUX_UE_TP2_API AMyCharacter : public ACharacter, public IInteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	UFUNCTION(BlueprintCallable)
	void Interact();

	UFUNCTION(BlueprintCallable)
	void Scroll(float Direction);

	UFUNCTION(BlueprintCallable)
	void StartHoldObj();

	UFUNCTION(BlueprintCallable)
	void ReleaseObj();

	UPROPERTY(EditDefaultsOnly)
	float SightLenght = 300.f;

	void CheckSight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//In protected
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Physic Handle")
	class UPhysicsHandleComponent* HandleCompo;

	FHitResult OutHit;
	FVector TargetLoc;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	AActor* ActorInSight = nullptr;
	void ResetActorInSight();
	bool IsImplementingInteractInterface(AActor* Actor);
};
