// Copyright Michael Stanley

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Components/Button.h"
#include "GameWinScreenUserWidget.h"
#include "GameJamGameGameMode.generated.h"


class UUserWidget;

UCLASS(minimalapi)
class AGameJamGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	AGameJamGameGameMode();

	// Called to see if the game is won
	void CheckWinCondition();
	// Called to set win cond
	void SetWinCondition(bool bHasWon);
	// Will be called when the Target is hit
	void IncrementScore();
	void GameWon();

	UFUNCTION(BlueprintCallable)
	void RestartGame();

	UFUNCTION(BlueprintCallable)
	void QuitGame();

	// Returns the time it took to win the game
	float GetWinTime();

	// Save
	void CreateSaveFile();
	void SaveGame();
	void LoadGame();

protected:
	// Win Screen Widget
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameWinScreenUserWidget> WinScreenWidgetClass;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* RestartButton;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* QuitButton;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* CompletionTimeTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// Score is the amount of targets hit
	int32 Score;
	// Total amount of targets in the level, and the score needed to win
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxScore;

private:
	bool bIsGameWon;
	float StartTime;
	float HighScore;

};



