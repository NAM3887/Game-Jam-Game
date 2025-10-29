// Copyright Michael Stanley

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetBall.generated.h"

UCLASS()
class GAMEJAMGAME_API ABallTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABallTarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UStaticMeshComponent* BallTargetMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UMaterialInstance* BallTargetMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UMaterialInstance* DamagedBallTargetMaterial;

	FTimerHandle DamageTimer;

	bool bIsDamaged;

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void OnTakeDamage();

	void ResetDamage();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
