// Copyright Michael Stanley

#include "GameJamGameGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "FirstPersonCharacterHud.h"
#include "GameFramework/HUD.h"
#include "SaveGameData.h"
#include "GameJamGameCharacter.h"

AGameJamGameGameMode::AGameJamGameGameMode() :
	Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	
	// Initialize Score and Win Variables
	Score = 0;
	MaxScore = 6; 
	HighScore = 99999.99f;
	bIsGameWon = false;
}
void AGameJamGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	StartTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
}
void AGameJamGameGameMode::CheckWinCondition()
{
	if (Score >= MaxScore)
	{
		SetWinCondition(true);
	}
}

void AGameJamGameGameMode::SetWinCondition(bool bHasWon)
{
	if (bHasWon)
	{
		GameWon();
	}
}

void AGameJamGameGameMode::IncrementScore()
{
	if (Score < MaxScore)
	{
		Score++;
	}
}

void AGameJamGameGameMode::GameWon()
{
	// Get Win Time
	float WinTime = GetWinTime();
	// Pause the game
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	// Load the High Score;
	LoadGame();
	// Save time, if it is a high score
	if ( WinTime < HighScore )
	{
		HighScore = WinTime;
		SaveGame();
	}
	// Show win UI (create a widget and add it to viewport)
	if (WinScreenWidgetClass)
	{
		UGameWinScreenUserWidget* WinWidget = CreateWidget<UGameWinScreenUserWidget>(GetWorld(), WinScreenWidgetClass);
		if (WinWidget)
		{
			WinWidget->AddToViewport();
			WinWidget->SetWinTimeText(WinTime);
			WinWidget->SetHighScoreText(HighScore);
		}
	}
	//Disable the player hud
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	AGameJamGameCharacter* PlayerCharacter = Cast<AGameJamGameCharacter>(PlayerController->GetPawn());
	if (PlayerCharacter)
	{
		PlayerCharacter->DisableHUD();
	}
	
}
void AGameJamGameGameMode::RestartGame()
{
	// Reset Start Time
	StartTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());

	// Open the level
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void AGameJamGameGameMode::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
}

float AGameJamGameGameMode::GetWinTime()
{
	float CurrentTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	return CurrentTime - StartTime;
}

void AGameJamGameGameMode::CreateSaveFile()
{
	USaveGameData* DataToSave = Cast<USaveGameData>(UGameplayStatics::CreateSaveGameObject(USaveGameData::StaticClass()));
	if (DataToSave)
	{
		DataToSave->WinTimeScores.Init(99999.99f, 5); // Initialize with a high default score
		UGameplayStatics::SaveGameToSlot(DataToSave, "Slot1", 0);
	}
}

void AGameJamGameGameMode::SaveGame()
{
	USaveGameData* DataToSave = Cast<USaveGameData>(UGameplayStatics::LoadGameFromSlot("Slot1", 0));

	if (DataToSave == nullptr)
	{
		DataToSave = Cast<USaveGameData>(UGameplayStatics::CreateSaveGameObject(USaveGameData::StaticClass()));
		if (DataToSave != nullptr)
		{
			// Initialize WinTimeScores array if necessary
			DataToSave->WinTimeScores.Init(99999.99f, 5); // Initialize with a high value
		}
	}

	// Add the current win time
	float CurrentWinTime = GetWinTime();
	DataToSave->WinTimeScores.Add(CurrentWinTime);

	// Sort and keep only the top 5 scores
	DataToSave->WinTimeScores.Sort();
	if (DataToSave->WinTimeScores.Num() > 5)
	{
		DataToSave->WinTimeScores.RemoveAt(5, DataToSave->WinTimeScores.Num() - 5);
	}

	UGameplayStatics::SaveGameToSlot(DataToSave, "Slot1", 0);
}
void AGameJamGameGameMode::LoadGame()
{
	USaveGameData* DataToLoad = Cast<USaveGameData>(UGameplayStatics::LoadGameFromSlot("Slot1", 0));
	if (DataToLoad != nullptr)
	{
		HighScore = DataToLoad->WinTimeScores.IsValidIndex(0) ? DataToLoad->WinTimeScores[0] : 99999.99f; // Get the top score
	}
	else if (!UGameplayStatics::DoesSaveGameExist("Slot1", 0))
	{
		CreateSaveFile();
	}

}
