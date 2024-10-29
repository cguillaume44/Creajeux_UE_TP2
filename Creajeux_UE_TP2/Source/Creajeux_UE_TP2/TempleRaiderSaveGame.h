// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TempleRaiderSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class CREAJEUX_UE_TP2_API UTempleRaiderSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	//create a transform variable called PlayerTransform
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FTransform PlayerTransform;

	//create a map called grabable max based on a guid and a transform
	UPROPERTY(VisibleAnywhere, Category = Basic)
	TMap<FGuid, FTransform> GrabableMax;

	//create a transform variable called PlayerTransform
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FRotator PlayerRotation;

	//create a map with GUID as key and a bool as value
	UPROPERTY(VisibleAnywhere, Category = Basic)
	TMap<FGuid, bool> DoorsStatus;

	//Saved Simon puzzle status
	UPROPERTY(VisibleAnywhere, Category = Basic)
	bool bSimonSolvedSave;

	//Saved treasure index
	UPROPERTY(VisibleAnywhere, Category = Basic)
	int32 TreasureIndexSave;

	//Create an array of GUID
	UPROPERTY(VisibleAnywhere, Category = Basic)
	TArray<FGuid> DestroyedPlatsSaved;

	//Saved plat sequence
	UPROPERTY(VisibleAnywhere, Category = Basic)
	int32 PlatSequenceSaved;

};
