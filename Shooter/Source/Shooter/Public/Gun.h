// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

class AProjectileBase;

UCLASS()
class SHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    void PullTrigger();

private:
    bool GetGunTrace(FHitResult& Hit, FVector& ShootDirection);
    AController* GetOwnerController() const;

private:
    UPROPERTY(VisibleAnywhere)
    USceneComponent* Root;

    UPROPERTY(VisibleAnywhere)
    USkeletalMeshComponent* Mesh;

    UPROPERTY(EditAnywhere)
    UParticleSystem* MuzzleFlash;
    UPROPERTY(EditAnywhere)
    USoundBase* MuzzleSound;

    UPROPERTY(EditAnywhere)
    UParticleSystem* ImpactFlash;
    UPROPERTY(EditAnywhere)
    USoundBase* ImpactSound;

    UPROPERTY(EditAnywhere)
    TSubclassOf<AProjectileBase> ProjectileClass;
    UPROPERTY(EditAnywhere)
    USceneComponent* ProjectileSpawnPoint;

    UPROPERTY(EditAnywhere)
    float MaxRange = 1000.0f;

    UPROPERTY(EditAnywhere)
    float Damage = 10.0f;
};
