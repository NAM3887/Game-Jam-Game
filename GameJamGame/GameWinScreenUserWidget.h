// Copyright Michael Stanley

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameWinScreenUserWidget.generated.h"


UCLASS(Abstract)
class GAMEJAMGAME_API UGameWinScreenUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* ContinueButton;
	UPROPERTY(EditAnywhere,meta= (BindWidget))
	class UButton* RestartButton;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* QuitButton;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* CompletionTimeTextBlock;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* HighScoreTextBlock;

public:	
	void SetWinTimeText(float WinTime);
	void SetHighScoreText(float HighScore);
	void LoadNextLevel();
};
