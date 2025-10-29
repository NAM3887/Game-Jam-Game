// Copyright Michael Stanley

#include "FirstPersonCharacterHud.h"
#include "Components/ProgressBar.h"


void UFirstPersonCharacterHUD::NativeConstruct()
{
    Super::NativeConstruct();

    // Load stamina widget
    if (StaminaWidgetClass)
    {
        UUserWidget* StaminaWidget = CreateWidget<UUserWidget>(GetWorld(), StaminaWidgetClass);
        if (StaminaWidget)
        {
            StaminaWidget->AddToViewport();
            StaminaBar = Cast<UProgressBar>(StaminaWidget->GetWidgetFromName(TEXT("StaminaBar")));
        }
    }
}

void UFirstPersonCharacterHUD::SetStaminaBar(float CurrentStamina, float MaxStamina)
{
	if (StaminaBar)
	{
		float StaminaPercentage = CurrentStamina / MaxStamina;
		StaminaBar->SetPercent(StaminaPercentage);
	}
}