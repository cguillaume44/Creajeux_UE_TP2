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

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void Interact();

	UFUNCTION(BlueprintCallable)
	void Scroll(float Direction);

	UFUNCTION(BlueprintCallable)
	void Grab();

	UFUNCTION(BlueprintCallable)
	void Release();

	UPROPERTY(EditDefaultsOnly, Category = "My chara")
	float HoldDistance = 200;

	UPROPERTY(EditDefaultsOnly, Category = "My chara")
	float SightLength = 400;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	class UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Physic Handle")
	class UPhysicsHandleComponent* HandleCompo;

	void UpdtadeHandleLocation();

	void CheckSight();

	bool CanGrabActor(FHitResult Hit);



private:
	bool bSavedPhysSim;
	FHitResult SightRaycast;
	FVector CamPos;
	AActor* ActorInSight = nullptr;
	void ResetActorInSight();
	bool IsImplementingInteractInterface(AActor* Actor);
};
