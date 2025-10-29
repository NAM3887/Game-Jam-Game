// Copyright Michael Stanley
#include "GameJamGameCharacter.h"
#include "GameJamGameProjectile.h"
#include "Animation/AnimInstance.h"
#include "FirstPersonCharacterHud.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"



DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AGameJamGameCharacter::AGameJamGameCharacter() :
	WalkSpeed(500.f),
	SprintSpeed(800.f),
	MaxStamina(500.f),
	CurrentStamina(MaxStamina),
	bIsSprinting(false),
	bIsStaminaRegenerating(false),
	StaminaRegenerationDelay(1.5f)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	// HUD, Note: we set to null to be paranoid programmers
	PlayerHUDClass = nullptr;
	PlayerHUD = nullptr;
}

void AGameJamGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerHUDClass)
	{
		PlayerHUD = CreateWidget<UFirstPersonCharacterHUD>(GetWorld(), PlayerHUDClass);
		if (PlayerHUD)
		{
			// Add the HUD to the ViewPort
			PlayerHUD->AddToViewport();
			// Set initial stamina in stamina bar
			PlayerHUD->SetStaminaBar(CurrentStamina, MaxStamina);
		}
	}
}

void AGameJamGameCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (PlayerHUD)
	{
		PlayerHUD->RemoveFromParent();
		// Garbage collecter will destroy
		PlayerHUD = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

void AGameJamGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGameJamGameCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGameJamGameCharacter::Look);

		// Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AGameJamGameCharacter::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AGameJamGameCharacter::StopSprint);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component!"), *GetNameSafe(this));
	}
}

void AGameJamGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update stamina based on current state
	if (bIsSprinting)
	{
		StaminaReduceFunction();
	}
	else
	{
		StaminaGainFunction();
	}
	// Update the HUD with current stamina
	if (PlayerHUD)
	{
		PlayerHUD->SetStaminaBar(CurrentStamina, MaxStamina);
	}
}

void AGameJamGameCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AGameJamGameCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AGameJamGameCharacter::Sprint()
{
	if (CurrentStamina > 0)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		bIsSprinting = true;
	}
}

void AGameJamGameCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	bIsSprinting = false;

	// Start cooldown for stamina regeneration
	bIsStaminaRegenerating = true;
	GetWorld()->GetTimerManager().SetTimer(StaminaRegenerationTimerHandle, 
		this, &AGameJamGameCharacter::ResetStaminaRegeneration, StaminaRegenerationDelay, false);
}

void AGameJamGameCharacter::ResetStaminaRegeneration()
{
	bIsStaminaRegenerating = false;
}

void AGameJamGameCharacter::StaminaGainFunction()
{
	if (CurrentStamina < MaxStamina && !bIsStaminaRegenerating)
	{
		CurrentStamina += 20 * GetWorld()->GetDeltaSeconds();
	}
}

void AGameJamGameCharacter::StaminaReduceFunction()
{
	if (bIsSprinting && CurrentStamina > 0)
	{
		CurrentStamina -= 10 * GetWorld()->GetDeltaSeconds();
	}
	else if (CurrentStamina <= 0)
	{
		StopSprint(); // Stop sprinting if stamina is depleted
	}
}

void AGameJamGameCharacter::DisableHUD()
{
	// disable the HUD
	if (PlayerHUD)
	{
		PlayerHUD->RemoveFromViewport();
	}
}