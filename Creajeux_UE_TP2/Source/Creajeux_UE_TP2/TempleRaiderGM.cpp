// Fill out your copyright notice in the Description page of Project Settings.


#include "TempleRaiderGM.h"
#include "Kismet/GameplayStatics.h"
#include "TempleRaiderSaveGame.h"
#include "Grabable.h"
#include "Door.h"
#include "Simon_Button.h"
#include "Simon_Manager.h"
#include "KeyTrigger.h"
#include "FallingPlat_Manager.h"
#include "FallingPlat.h"


void ATempleRaiderGM::BeginPlay()
{
	Super::BeginPlay();
	
	SaveName=CreateSaveName();
	TryToLoad();

}

FString ATempleRaiderGM::CreateSaveName()
{
	FString TempString;
	//get current level name and save it in the save name
	TempString = GetWorld()->GetMapName();
	//remove the path and the extension
	TempString.RemoveFromStart("/Game/Maps/");
	TempString.RemoveFromEnd(".umap");
	//add the an extension
	TempString.Append("-Player1");

	return TempString;
}

#pragma region SaveProcess

void ATempleRaiderGM::TryToSave()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveName, 0))
	{
		UTempleRaiderSaveGame* SaveGameInstance = Cast<UTempleRaiderSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveName, 0));
		SaveGameInstance->PlayerTransform = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorTransform();
		SaveGameInstance->GrabableMax.Empty();
		SaveGameInstance->DoorsStatus.Empty();

		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            SaveGameInstance->PlayerRotation = PlayerController->GetControlRotation();
        }

        SaveGameInstance->bSimonSolvedSave = bSimonSolved;
        SaveGameInstance->TreasureIndexSave = TreasureIndex;

		// ############################# Cache all actors of interest  #############################
		TArray<AActor*> Grabables, Doors, FallingPlatManagers;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGrabable::StaticClass(), Grabables);
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADoor::StaticClass(), Doors);
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFallingPlat_Manager::StaticClass(), FallingPlatManagers);

		// ############################# Process Grabables  ############################# 
		for (AActor* Actor : Grabables)
		{
			AGrabable* Grabable = Cast<AGrabable>(Actor);
			if (Grabable && !SaveGameInstance->GrabableMax.Contains(Grabable->GrabGUID))
			{
				SaveGameInstance->GrabableMax.Add(Grabable->GrabGUID, Grabable->GetActorTransform());
			}
			else if (GEngine && Actor)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Duplicate object GUID found, check logs"));
				UE_LOG(LogTemp, Error, TEXT("Duplicate object GUID found on: %s"), *Actor->GetName());
			}
		}

		//  ############################# Process Doors  ############################# 
		for (AActor* Actor : Doors)
		{
			ADoor* Door = Cast<ADoor>(Actor);
			if (Door && !SaveGameInstance->DoorsStatus.Contains(Door->DoorGUID))
			{
				SaveGameInstance->DoorsStatus.Add(Door->DoorGUID, Door->bDoorOpen);
			}
			else if (GEngine && Actor)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Duplicate door GUID found, check logs"));
				UE_LOG(LogTemp, Error, TEXT("Duplicate door GUID found on: %s"), *Actor->GetName());
			}
		}

		//  ############################# Process FallingPlatManagers  ############################# 
		for (AActor* Actor : FallingPlatManagers)
		{
			AFallingPlat_Manager* FallingPlatManager = Cast<AFallingPlat_Manager>(Actor);
			if (FallingPlatManager)
			{
				SaveGameInstance->PlatSequenceSaved = FallingPlatManager->RandomSequenceIndex;
			}
		}
		//Updated by the platform being destroyed directly
        //Since we assign the array directly we don't need to clean it.
		SaveGameInstance->DestroyedPlatsSaved = DestroyedPlats;

        //  ############################# Save Process done  ############################# 

		UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveName, 0);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "Save overwritten");
	}
	else
	{
		CreateSave();
	}
}
#pragma endregion

#pragma region LoadProcess



void ATempleRaiderGM::TryToLoad()
{
    if (UGameplayStatics::DoesSaveGameExist(SaveName, 0))
    {
        UTempleRaiderSaveGame* SaveGameInstance = Cast<UTempleRaiderSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveName, 0));
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController && PlayerController->GetPawn())
        {
			PlayerController->GetPawn()->SetActorTransform(SaveGameInstance->PlayerTransform);
			PlayerController->SetControlRotation(SaveGameInstance->PlayerRotation);
        }

        // ############################# Cache all actors of interest  #############################
        TArray<AActor*> Grabables, Doors, FallingPlatManagers, SimonButtons, SimonManagers, KeyTriggers, FallingPlats;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGrabable::StaticClass(), Grabables);
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADoor::StaticClass(), Doors);
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFallingPlat_Manager::StaticClass(), FallingPlatManagers);
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASimon_Button::StaticClass(), SimonButtons);
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASimon_Manager::StaticClass(), SimonManagers);
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AKeyTrigger::StaticClass(), KeyTriggers);
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFallingPlat::StaticClass(), FallingPlats);

        // ############################# Process Grabables #############################
        for (AActor* Actor : Grabables)
        {
            AGrabable* Grabable = Cast<AGrabable>(Actor);
            if (Grabable)
            {
                FTransform* GrabableTransform = SaveGameInstance->GrabableMax.Find(Grabable->GrabGUID);
                if (GrabableTransform)
                {
                    Grabable->SetActorTransform(*GrabableTransform);
                }
            }
        }

        // ############################# Process Doors #############################
        for (AActor* Actor : Doors)
        {
            ADoor* Door = Cast<ADoor>(Actor);
            if (Door)
            {
                bool* bDoorStatus = SaveGameInstance->DoorsStatus.Find(Door->DoorGUID);
                if (bDoorStatus)
                {
                    if (*bDoorStatus)
                    {
                        Door->SkipOpendoor();
                        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Door open"));
                    }
                    else
                    {
                        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Door closed"));
                    }
                }
            }
        }

        // ############################# Process Simon Puzzle #############################
        bSimonSolved = SaveGameInstance->bSimonSolvedSave;
        if (bSimonSolved)
        {
            for (AActor* Actor : SimonButtons)
            {
                if (Actor)
                {
                    Actor->Destroy();
                }
            }
            for (AActor* Actor : SimonManagers)
            {
                if (Actor)
                {
                    Actor->Destroy();
                }
            }
        }

        // ############################# Process Treasure #############################
        TreasureIndex = SaveGameInstance->TreasureIndexSave;
        for (AActor* Actor : KeyTriggers)
        {
            AKeyTrigger* KeyTrigger = Cast<AKeyTrigger>(Actor);
            if (KeyTrigger)
            {
                //Process if we saved with the chest door open
                //Will not scale well
                if (TreasureIndex >= 4)
                {
                    KeyTrigger->Destroy();
                    TArray<AActor*> Keys;
                    UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Key", Keys);
                    for (AActor* Key : Keys)
                    {
                        if (Key)
                        {
                            Key->Destroy();
                        }
                    }
                }
                else
                {
                    KeyTrigger->KeyCount = TreasureIndex;
                }
            }
        }

        // ############################# Process FallingPlatManagers #############################
        for (AActor* Actor : FallingPlatManagers)
        {
            AFallingPlat_Manager* FallingPlatManager = Cast<AFallingPlat_Manager>(Actor);
            if (FallingPlatManager)
            {
                FallingPlatManager->UpdateSequenceIndex(SaveGameInstance->PlatSequenceSaved);
            }
        }

        // ############################# Process FallingPlats #############################
        DestroyedPlats = SaveGameInstance->DestroyedPlatsSaved;
        for (AActor* Actor : FallingPlats)
        {
            AFallingPlat* FallingPlat = Cast<AFallingPlat>(Actor);
            if (FallingPlat && DestroyedPlats.Contains(FallingPlat->PlatGUID))
            {
                FallingPlat->Destroy();
            }
        }

        //  ############################# Load Process done  ############################# 

        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "Save loaded");
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "Save does not exist");
    }
}
#pragma endregion

void ATempleRaiderGM::DeleteSave()
{
	//check if the save exists
	if (UGameplayStatics::DoesSaveGameExist(SaveName, 0))
	{
		//delete the save
		UGameplayStatics::DeleteGameInSlot(SaveName, 0);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "Save deleted");
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "Save does not exist");
	}
}


void ATempleRaiderGM::CreateSave()
{
	//create a new save game object
	UTempleRaiderSaveGame* SaveGameInstance = Cast<UTempleRaiderSaveGame>(UGameplayStatics::CreateSaveGameObject(UTempleRaiderSaveGame::StaticClass()));
	//save the game
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveName, 0);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "Save created");
	TryToSave();
}