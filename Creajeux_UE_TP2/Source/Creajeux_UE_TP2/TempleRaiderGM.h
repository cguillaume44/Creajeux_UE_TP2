// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TempleRaiderGM.generated.h"

/**
 * 
 */
UCLASS()
class CREAJEUX_UE_TP2_API ATempleRaiderGM : public AGameModeBase
{
	GENERATED_BODY()
public:

	//create a ufunction bp callable called try to save
	UFUNCTION(BlueprintCallable)
	void TryToSave();

	//create a ufunction bp callable called try to save
	UFUNCTION(BlueprintCallable)
	void TryToLoad();

	//create a ufunction bp callable called try to save
	UFUNCTION(BlueprintCallable)
	void DeleteSave();

	//Saved Simon puzzle status
	UPROPERTY(EditAnywhere)
	bool bSimonSolved=false;

	//Saved treasure index
	UPROPERTY(EditAnywhere)
	int32 TreasureIndex=0;

	//Saved treasure index
	UPROPERTY(EditAnywhere)
	TArray<FGuid> DestroyedPlats;

	//Saved treasure index
	UPROPERTY(EditAnywhere)
	int32 PlatSequence;

protected:
	//call begin play
	virtual void BeginPlay() override;

	void CreateSave();

	void loadProcess();

	FString SaveName="Save1";

	void CreateSaveName();
};
