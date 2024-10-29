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
	
	CreateSaveName();
	TryToLoad();

}

void ATempleRaiderGM::loadProcess()
{
}

void ATempleRaiderGM::CreateSaveName()
{
	FString TempString;
	//get current level name and save it in the save name
	TempString = GetWorld()->GetMapName();
	//remove the path and the extension
	TempString.RemoveFromStart("/Game/Maps/");
	TempString.RemoveFromEnd(".umap");
	//add the an extension
	TempString.Append("-Player1");

	SaveName = TempString;
	//display the save name on screen
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, SaveName);
}

#pragma region SaveProcess

void ATempleRaiderGM::TryToSave()
{
	//check if the save exists
	if (UGameplayStatics::DoesSaveGameExist(SaveName, 0))
	{
		//Load the save game object
		UTempleRaiderSaveGame* SaveGameInstance = Cast<UTempleRaiderSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveName, 0));
		
		//Set all the data to the save game object
		SaveGameInstance->PlayerTransform = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorTransform();

		SaveGameInstance->GrabableMax.Empty();
		SaveGameInstance->DoorsStatus.Empty();

#pragma region Grabable save



		//get all the grabable actors of class grabable

		TArray<AActor*> Grabables;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGrabable::StaticClass(), Grabables);
		//for each grabable actor
		for (AActor* Actor : Grabables)
		{
			//cast the actor to a grabable actor
			AGrabable* Grabable = Cast<AGrabable>(Actor);

			if (!SaveGameInstance->GrabableMax.Contains(Grabable->GrabGUID))
			{
				//add the actor to the save game object
				SaveGameInstance->GrabableMax.Add(Grabable->GrabGUID, Grabable->GetActorTransform());
			}
			else
			{
				if (GEngine && Actor)
				{
					//Display the actor name in log

					if (Actor)
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Duplicate object GUID found, check logs"));
						UE_LOG(LogTemp, Error, TEXT("Duplicate object GUID found on: %s"), *Actor->GetName());
					}

				}
			}
		}
#pragma endregion
		//get the controller rotation and save it in the save game object
		SaveGameInstance->PlayerRotation = GetWorld()->GetFirstPlayerController()->GetControlRotation();

		//Save the simon puzzle status
		SaveGameInstance->bSimonSolvedSave = bSimonSolved;

		//Save the treasure index
		SaveGameInstance->TreasureIndexSave = TreasureIndex;

#pragma region DoorSave



		//get all the grabable actors of class grabable

		TArray<AActor*> Doors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADoor::StaticClass(), Doors);
		//for each grabable actor
		for (AActor* Actor : Doors)
		{
			//cast the actor to a grabable actor
			ADoor* Door = Cast<ADoor>(Actor);

			if (!SaveGameInstance->DoorsStatus.Contains(Door->DoorGUID))
			{
				//add the actor to the save game object
				SaveGameInstance->DoorsStatus.Add(Door->DoorGUID, Door->bDoorOpen);
			}
			else
			{
				if (GEngine && Actor)
				{
					//Display the actor name in log

					if (Actor)
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Duplicate door GUID found, check logs"));
						UE_LOG(LogTemp, Error, TEXT("Duplicate door GUID found on: %s"), *Actor->GetName());
					}

				}
			}
		}

#pragma endregion

#pragma region Falling plat save

		//get first falling plat manager
		TArray<AActor*> FallingPlatManagers;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFallingPlat_Manager::StaticClass(), FallingPlatManagers);
		//for each falling plat manager
		for (AActor* Actor : FallingPlatManagers)
		{
			//cast the actor to a falling plat manager
			AFallingPlat_Manager* FallingPlatManager = Cast<AFallingPlat_Manager>(Actor);
			//set the falling plat manager sequence index to the save game object
			SaveGameInstance->PlatSequenceSaved = FallingPlatManager->RandomSequenceIndex;
		}

		//Save the destroyed plats array
		SaveGameInstance->DestroyedPlatsSaved = DestroyedPlats;

#pragma endregion


		//save the game
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveName, 0);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "Save ovewritten");
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
	//check if the save exists
	if (UGameplayStatics::DoesSaveGameExist(SaveName, 0))
	{
		//Load the save game object
		UTempleRaiderSaveGame* SaveGameInstance = Cast<UTempleRaiderSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveName, 0));
		//set the player transform to the saved transform
		GetWorld()->GetFirstPlayerController()->GetPawn()->SetActorTransform(SaveGameInstance->PlayerTransform);

		//set the player rotation to the saved rotation
		GetWorld()->GetFirstPlayerController()->SetControlRotation(SaveGameInstance->PlayerRotation);

#pragma region Grabable load



		TArray<AActor*> Grabables;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGrabable::StaticClass(), Grabables);
		//for each grabable actor
		for (AActor* Actor : Grabables)
		{
			//cast the actor to a grabable actor
			AGrabable* Grabable = Cast<AGrabable>(Actor);
			//find the grabable guid in the save game object map
			FTransform* GrabableTransform = SaveGameInstance->GrabableMax.Find(Grabable->GrabGUID);
			//if the grabable guid is found
			if (GrabableTransform)
			{
				//set the grabable actor transform to the saved transform
				Grabable->SetActorTransform(*GrabableTransform);
			}

		}
#pragma endregion

#pragma region DoorLoad

		TArray<AActor*> Doors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADoor::StaticClass(), Doors);
		//for each grabable actor
		for (AActor* Actor : Doors)
		{
			//cast the actor to a grabable actor
			ADoor* Door = Cast<ADoor>(Actor);
			//find the grabable guid in the save game object map
			bool bDoorStatus = *SaveGameInstance->DoorsStatus.Find(Door->DoorGUID);
			//if the grabable guid is found

			//log the door status bool  value on screen
			if (GEngine && Actor)
			{
				//Display the actor name in log
				if (bDoorStatus)
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

#pragma endregion

#pragma region SimonLoad




		//load the simon puzzle status
		bSimonSolved = SaveGameInstance->bSimonSolvedSave;

		if (bSimonSolved)
		{
			//get all simonpuzzle actor and destroy them
			TArray<AActor*> SimonButtons;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASimon_Button::StaticClass(), SimonButtons);
			for (AActor* Actor : SimonButtons)
			{
				if (Actor)
				{
					Actor->Destroy();
				}
			}
			//get simon manager and destroy it
			TArray<AActor*> SimonManagers;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASimon_Manager::StaticClass(), SimonManagers);
			for (AActor* Actor : SimonManagers)
			{
				if (Actor)
				{
					Actor->Destroy();
				}
			}
		}
#pragma endregion

#pragma region TreasureLoad




		//load the treasure index
		TreasureIndex = SaveGameInstance->TreasureIndexSave;

		//get the keytrigger actor and set the keycount to the treasure index
		TArray<AActor*> KeyTriggers;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AKeyTrigger::StaticClass(), KeyTriggers);
		for (AActor* Actor : KeyTriggers)
		{
			AKeyTrigger* KeyTrigger = Cast<AKeyTrigger>(Actor);
			if (KeyTrigger)
			{
				if (TreasureIndex>=4)
				{
					KeyTrigger->Destroy();
					//get all actor with tags "key" and destroy them
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
#pragma endregion

#pragma region FallingPlatLoad

		//get first falling plat manager
		TArray<AActor*> FallingPlatManagers;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFallingPlat_Manager::StaticClass(), FallingPlatManagers);
		//for each falling plat manager
		for (AActor* Actor : FallingPlatManagers)
		{
			//cast the actor to a falling plat manager
			AFallingPlat_Manager* FallingPlatManager = Cast<AFallingPlat_Manager>(Actor);
			//set the falling plat manager sequence index to the save game object
			FallingPlatManager->UpdateSequenceIndex(SaveGameInstance->PlatSequenceSaved);
		}

		//Load the destroyed plats array
		DestroyedPlats = SaveGameInstance->DestroyedPlatsSaved;

		//get all the falling plat actors of class falling plat
		TArray<AActor*> FallingPlats;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFallingPlat::StaticClass(), FallingPlats);
		//for each falling plat actor
		for (AActor* Actor : FallingPlats)
		{
			//cast the actor to a falling plat actor
			AFallingPlat* FallingPlat = Cast<AFallingPlat>(Actor);
			//if the destroyed plats array contains the falling plat guid
			if (DestroyedPlats.Contains(FallingPlat->PlatGUID))
			{
				//destroy the actor
				FallingPlat->Destroy();
			}
		}

		//LoadProcessOver
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "Save loaded");
	}
	else
	{
		//Save DO NOT EXIST do nothing
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