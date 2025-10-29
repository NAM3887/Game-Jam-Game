// Copyright Michael Stanley

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "GameJamGameCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AGameJamGameCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// clean up pawn when game ends
	virtual void EndPlay(const EEndPlayReason::Type EndPLayReason) override;

public:
	// Sets Default Values for this characters properties
	AGameJamGameCharacter();

	// Disables the player hud (stamina, crosshair, timer)
	void DisableHUD();

	// Walk Speed
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed;

	// Run Speed
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed;

	// Max stamina
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float MaxStamina;

	// Curent Stamina
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float CurrentStamina;

	// Used to see if the character is currently sprinting
	UPROPERTY()
	bool bIsSprinting;

	bool bIsStaminaRegenerating;
	// Time in seconds before stamina starts to refill
	float StaminaRegenerationDelay;

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	// Called Every Frame
	virtual void Tick(float DeltaTime) override;
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

protected:
	
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	
	// Widget class to spawn for the heads up display
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UFirstPersonCharacterHUD> PlayerHUDClass;
	// The widget instance for the HUD
	UPROPERTY()
	class UFirstPersonCharacterHUD* PlayerHUD;

	// Timer for stamina
	FTimerHandle StaminaRegenerationTimerHandle;

	// Movement and Look function Declarations
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Sprint();
	void StopSprint();
	void ResetStaminaRegeneration();

	// Called in tick to decrement stamina while running or increment when the delay cool down is over
	void StaminaGainFunction();
	void StaminaReduceFunction();
	
};

