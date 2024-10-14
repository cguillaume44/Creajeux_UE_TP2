// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Simon_Manager.generated.h"

UCLASS()
class CREAJEUX_UE_TP2_API ASimon_Manager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASimon_Manager();

	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly, Category = "Simon Manager")
	UBillboardComponent* Billboard;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class ASimon_Button* LastButtonPressed;
	bool bIsPlayingSequence;
	int32 GuessIndex = 0;
	int32 MainSequenceIndex = 0;
	int32 PreviewSequenceIndex = 0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, Category = "Simon Button")
	class ASimon_Button* ResetBtn;

	//SEQUENCE 1
	UPROPERTY(EditAnywhere, Category = "Simon Button")
	TArray<class ASimon_Button*> Sequence1;

	//SEQUENCE 2
	UPROPERTY(EditAnywhere, Category = "Simon Button")
	TArray<class ASimon_Button*> Sequence2;

	//SEQUENCE 3
	UPROPERTY(EditAnywhere, Category = "Simon Button")
	TArray<class ASimon_Button*> Sequence3;

	void ButtonPressed(class ASimon_Button* Button);

	void PlaySequence();

	void checkSequence();

	void TriggerWinProcess();

	//Helper Function that return a ASimon_button pointer array depending on the sequence number
	TArray<class ASimon_Button*> GetSequence(int32 SequenceNumber);

	//Pointer to a door actor object in the level
	UPROPERTY(EditAnywhere, Category = "Simon")
	class ADoor* DoorActor;

};
