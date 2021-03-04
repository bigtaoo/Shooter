// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "kismet/GameplayStatics.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
    ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
    RootComponent = ProjectileMesh;

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
    ProjectileMovement->InitialSpeed = MoveSpeed;
    ProjectileMovement->MaxSpeed = MoveSpeed;

    InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    UE_LOG(LogTemp, Warning, TEXT("Projectile on hit"));
    AActor* myOwner = GetOwner();
    if (myOwner == nullptr)
    {
        return;
    }

    if (OtherActor != nullptr && OtherActor != this && OtherActor != myOwner)
    {
        UE_LOG(LogTemp, Warning, TEXT("Apply damage: %f"), Damage);
        UGameplayStatics::ApplyDamage(OtherActor, Damage, myOwner->GetInstigatorController(), this, DamageType);
    }

    Destroy();
}
