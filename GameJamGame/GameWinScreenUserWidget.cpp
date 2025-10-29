// Copyright Michael Stanley

#include "GameWinScreenUserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"


// display arr 
void UGameWinScreenUserWidget::SetWinTimeText(float WinTime)
{
	FString WinTimeString = FString::Printf(TEXT("Time: %.2f"), WinTime);
	// Update win time
	if (CompletionTimeTextBlock)
	{
		CompletionTimeTextBlock->SetText(FText::FromString(WinTimeString));
	}
}

void UGameWinScreenUserWidget::SetHighScoreText(float HighScore)
{
	FString HighScoreString = FString::Printf(TEXT("PB: %.2f"), HighScore);
	// Update high score time
	if (HighScoreTextBlock)
	{
		HighScoreTextBlock->SetText(FText::FromString(HighScoreString));
	}
}

void UGameWinScreenUserWidget::LoadNextLevel()
{
	// Open the level
	UGameplayStatics::OpenLevel(this, FName("Level_Two"), false);
}
