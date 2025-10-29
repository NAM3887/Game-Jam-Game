// Copyright Michael Stanley

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FirstPersonCharacterHUD.generated.h"

UCLASS(Abstract)
class GAMEJAMGAME_API UFirstPersonCharacterHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	// Update HUD with current stamina
	UFUNCTION(BlueprintCallable)
	void SetStaminaBar(float CurrentStamina, float MaxStamina);

protected:
	// Widget to display the Current Stamina
	 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> StaminaWidgetClass;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UProgressBar* StaminaBar;

};
