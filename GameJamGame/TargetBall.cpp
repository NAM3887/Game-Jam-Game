// Copyright Michael Stanley


#include "TargetBall.h"
#include "Kismet/GameplayStatics.h"
#include "GameJamGameProjectile.h"
#include "GameJamGameGameMode.h"

// Sets default values
ABallTarget::ABallTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BallTargetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallTargetMesh"));
	DamagedBallTargetMaterial = CreateDefaultSubobject<UMaterialInstance>(TEXT("DamageMaterial"));
	BallTargetMaterial = CreateDefaultSubobject<UMaterialInstance>(TEXT("BallTargetMaterial"));

	bIsDamaged = false;

	// BallTargetMesh->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void ABallTarget::BeginPlay()
{
	// Super::BeginPlay();
	BallTargetMesh->OnComponentHit.AddDynamic(this, &ABallTarget::OnComponentHit);
}

// Called every frame
void ABallTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ABallTarget::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (AGameJamGameProjectile* HitActor = Cast<AGameJamGameProjectile>(OtherActor))
	{
		UGameplayStatics::ApplyDamage(this, 20.0f, nullptr, OtherActor, UDamageType::StaticClass());
		OnTakeDamage();
		bIsDamaged = true;
	}
}
void ABallTarget::OnTakeDamage()
{
	BallTargetMesh->SetMaterial(0, DamagedBallTargetMaterial);

	if (!bIsDamaged)
	{
		// Increment score
		if (AGameJamGameGameMode* GameMode = Cast<AGameJamGameGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			GameMode->AGameJamGameGameMode::IncrementScore();
		}
	}

	/* // This is the timer for the ball to turn back can be used for a timed target!!!!!!!!!!!!!!!!!!!!
	GetWorld()->GetTimerManager().SetTimer(DamageTimer, this, &ABallTarget::ResetDamage, 1.5f, false);*/
}

void ABallTarget::ResetDamage()
{
	BallTargetMesh->SetMaterial(0, BallTargetMaterial);
}
