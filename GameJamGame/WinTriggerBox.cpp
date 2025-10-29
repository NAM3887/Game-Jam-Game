// Copyright Michael Stanley


#include "WinTriggerBox.h"

#include "Kismet/GameplayStatics.h"
#include "GameJamGameGameMode.h"

AWinTriggerBox::AWinTriggerBox()
{
	// Bind the overlap event
	OnActorBeginOverlap.AddDynamic(this, &AWinTriggerBox::OnPlayerOverlap);
}

void AWinTriggerBox::BeginPlay()
{
	Super::BeginPlay();
}

void AWinTriggerBox::OnPlayerOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	// Check that its not triggering itself
	if (OverlappedActor && (OtherActor != this))
	{
		// Get the Game Mode and call the IncrementScore function
		if (AGameJamGameGameMode* GameMode = Cast<AGameJamGameGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			// When the player enters the WinTrigger Check if they can win
			GameMode->AGameJamGameGameMode::CheckWinCondition();
		}
	}
}

