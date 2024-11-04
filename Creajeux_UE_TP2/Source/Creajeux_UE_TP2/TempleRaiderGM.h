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

	//################ SAVE SETTINGS ################

	UFUNCTION(BlueprintCallable)
	void TryToSave();

	UFUNCTION(BlueprintCallable)
	void TryToLoad();

	UFUNCTION(BlueprintCallable)
	void DeleteSave();

	UPROPERTY(EditAnywhere)
	bool bSimonSolved=false;

	UPROPERTY(EditAnywhere)
	int32 TreasureIndex=0;

	UPROPERTY(EditAnywhere)
	TArray<FGuid> DestroyedPlats;

	UPROPERTY(EditAnywhere)
	int32 PlatSequence;

	//################ QUALITY SETTINGS ################
	UFUNCTION(BlueprintCallable, Category = "Quality settings")
	void SetQuality(int32 Quality=1);

	UFUNCTION(BlueprintCallable, Category = "Quality settings")
	void DisplayFramerateStats(bool bDisplay);

	UFUNCTION(BlueprintCallable, Category = "Quality settings")
	void RunQualityBenchmark();

protected:
	//call begin play
	virtual void BeginPlay() override;

	//################ SAVE SETTINGS ################
	void CreateSave();

	FString SaveName="Save1";

	FString CreateSaveName();
};
