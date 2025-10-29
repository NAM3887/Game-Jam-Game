// Copyright Michael Stanley

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "GameJamGameGameMode.h"
#include "WinTriggerBox.generated.h"

// Forward Declaration
class AGameJamGameGameMode;

UCLASS()
class GAMEJAMGAME_API AWinTriggerBox : public ATriggerBox
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	AWinTriggerBox();

	UFUNCTION()
	void OnPlayerOverlap(AActor* OverlappedActor, AActor* OtherActor);

};
