// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class CREAJEUX_UE_TP2_API AMyCharacter : public ACharacter
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
	void Grab();

	UFUNCTION(BlueprintCallable)
	void Release();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//In protected
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* CameraComponent;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
