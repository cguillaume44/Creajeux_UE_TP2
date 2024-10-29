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
	UPROPERTY(VisibleAnywhere)
	FTransform PlayerTransform;

	//create a map called grabable max based on a guid and a transform
	UPROPERTY(VisibleAnywhere)
	TMap<FGuid, FTransform> GrabableMax;

	//create a transform variable called PlayerTransform
	UPROPERTY(VisibleAnywhere)
	FRotator PlayerRotation;

	//create a map with GUID as key and a bool as value
	UPROPERTY(VisibleAnywhere)
	TMap<FGuid, bool> DoorsStatus;

	//Saved Simon puzzle status
	UPROPERTY(VisibleAnywhere)
	bool bSimonSolvedSave;

	//Saved treasure index
	UPROPERTY(VisibleAnywhere)
	int32 TreasureIndexSave;

	//Create an array of GUID
	UPROPERTY(VisibleAnywhere)
	TArray<FGuid> DestroyedPlatsSaved;

	//Saved plat sequence
	UPROPERTY(VisibleAnywhere)
	int32 PlatSequenceSaved;

};
