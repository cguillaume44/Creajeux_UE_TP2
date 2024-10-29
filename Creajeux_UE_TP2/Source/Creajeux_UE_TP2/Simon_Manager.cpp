// Fill out your copyright notice in the Description page of Project Settings.


#include "Simon_Manager.h"
#include "Components/BillboardComponent.h"
#include "Simon_Button.h"
#include "Kismet/GameplayStatics.h"
#include "Door.h"
#include "TempleRaiderGM.h"

// Sets default values
ASimon_Manager::ASimon_Manager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the root component
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	//attach a billboard component to the root component
	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetupAttachment(RootComponent);


}

// Called when the game starts or when spawned
void ASimon_Manager::BeginPlay()
{
	Super::BeginPlay();

	//get all actor of class simon_button and for each of them, set the manager to this
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASimon_Button::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		if (Actor)
		{
			ASimon_Button* Button = Cast<ASimon_Button>(Actor);
			Button->Manager = this;
		}

	}
	//if sequence 1 is empty, throw a screen message
	if (!ResetBtn)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Reset btn not set"));
	}
	//if sequence 1 is empty, throw a screen message
	if (Sequence1.Num() == 0 || Sequence1.Contains(ResetBtn))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Sequence 1 is empty or contain reset btn"));
	}
	//if sequence 2 is empty, throw a screen message
	if (Sequence2.Num() == 0 || Sequence2.Contains(ResetBtn))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Sequence 2 is empty  or contain reset btn"));
	}
	//if sequence 3 is empty, throw a screen message
	if (Sequence3.Num() == 0 || Sequence3.Contains(ResetBtn))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Sequence 3 is empty  or contain reset btn"));
	}
	
}

// Called every frame
void ASimon_Manager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Called by the button when it is pressed
void ASimon_Manager::ButtonPressed(ASimon_Button* Button)
{
	if (!bIsPlayingSequence && Button && ResetBtn)
	{
		LastButtonPressed = Button;
		if (LastButtonPressed == ResetBtn)
		{
			GuessIndex = 0;
			PlaySequence();
		}
		else
		{
			checkSequence();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InvalidCondition button pressed"));
	}

}

void ASimon_Manager::PlaySequence()
{
	bIsPlayingSequence = true;
	TArray<class ASimon_Button*> CurrentSeq = GetSequence(MainSequenceIndex);
	if (CurrentSeq.IsValidIndex(0) && CurrentSeq.IsValidIndex(PreviewSequenceIndex))
	{
		ASimon_Button* Button = CurrentSeq[PreviewSequenceIndex];
		if (Button)
		{
			Button->LightUp();
			PreviewSequenceIndex++;
			FTimerHandle LightDownTimer;
			//call light down function inside Simon_Button on the button after 0.3 sec
			GetWorldTimerManager().SetTimer(LightDownTimer, [Button]() { Button->LightDown(); }, 0.3f, false);

			//call play sequence again after 0.8 sec
			FTimerHandle PlaySequenceTimer;
			GetWorldTimerManager().SetTimer(PlaySequenceTimer, this, &ASimon_Manager::PlaySequence, 0.8f, false);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("InvalidCondition playSequence"));

		}

	}
	else
	{
		//Preview sequence completed
		PreviewSequenceIndex = 0;
		bIsPlayingSequence = false;
	}

}

void ASimon_Manager::checkSequence()
{
	if (GetSequence(MainSequenceIndex).IsValidIndex(GuessIndex))
	{
		ASimon_Button* Button = GetSequence(MainSequenceIndex)[GuessIndex];
		if (Button && Button == LastButtonPressed)
		{
			GuessIndex++;
			UE_LOG(LogTemp, Warning, TEXT("Good guess"));

			//get the last index of getsequence(mainsequenceindex) and if the guess index is equal to it, increment the mainsequenceindex and reset the guess index
			if (GuessIndex > GetSequence(MainSequenceIndex).Num()-1)
			{
				MainSequenceIndex++;
				GuessIndex = 0;
				UE_LOG(LogTemp, Warning, TEXT("Sequence complete"));

				//Check if we are done with the 3 sequences
				//hardcoded so bad, if this system need to scale up, a refactor is needed.
				//The refactor could be a sequence struct or map with the sequence number and the array of buttons
				//good enough for a prototype, assume communication with the designer
				if (MainSequenceIndex>2)
				{
					//wait 0.5 sec before triggering the win process
					FTimerHandle WinTimer;
					GetWorldTimerManager().SetTimer(WinTimer, this, &ASimon_Manager::TriggerWinProcess, 0.5f, false);
				}
			}

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Bad guess"));
			MainSequenceIndex = 0;
			GuessIndex = 0;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InvalidCondition checkSequence"));
	}

}

void ASimon_Manager::TriggerWinProcess()
{
	UE_LOG(LogTemp, Warning, TEXT("WIN!!!!"));
	
	//get the game mode of class TempleRaiderGM and set the simon solved to true
	ATempleRaiderGM* GM = Cast<ATempleRaiderGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->bSimonSolved = true;
	}

	//destroy all the simon_button and then destroy the manager
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASimon_Button::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		if (Actor)
		{
			ASimon_Button* Button = Cast<ASimon_Button>(Actor);
			Button->Destroy();
		}

	}
	//Send the message to open the door.
	if (DoorActor)
	{
		DoorActor->CheckOpenDoor();
	}

	Destroy();

}

TArray<class ASimon_Button*> ASimon_Manager::GetSequence(int32 SequenceNumber)
{
	//Switch on the sequence number and return the corresponding sequence
	//Send an empty array if the sequence number is invalid
	switch (SequenceNumber)
	{
	case 0:
		return Sequence1;
		break;
	case 1:
		return Sequence2;
		break;
	case 2:
		return Sequence3;
		break;
	default:
		return TArray<class ASimon_Button*>();
		break;
	}
}


