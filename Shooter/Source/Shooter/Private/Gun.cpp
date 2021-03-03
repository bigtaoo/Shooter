// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "kismet/GameplayStatics.h"
#include "DrawDebughelpers.h"
#include "ProjectileBase.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(Root);

    ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
    ProjectileSpawnPoint->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::PullTrigger()
{
    UGameplayStatics::SpawnEmitterAttached(MuzzleFlash,Mesh, TEXT("MuzzleFlashSocket"));
    UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

    if (ProjectileClass)
    {
        auto ownerPawn = Cast<APawn>(GetOwner());
        if (ownerPawn == nullptr)
        {
            return;
        }
        auto tempProjectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass,
            ProjectileSpawnPoint->GetComponentLocation(),
            ownerPawn->GetActorRotation());
        tempProjectile->SetOwner(this);
    }

    FHitResult result;
    FVector shotDirection;
    if (GetGunTrace(result, shotDirection))
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactFlash, result.Location, shotDirection.Rotation());
        UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, result.Location, shotDirection.Rotation());

        auto target = result.GetActor();
        if (target != nullptr)
        {
            // UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *target->GetName());

            FPointDamageEvent damageEvent(Damage, result, shotDirection, nullptr);
            target->TakeDamage(Damage, damageEvent, GetOwnerController(), this);
        }
    }
}

AController* AGun::GetOwnerController() const
{
    auto ownerPawn = Cast<APawn>(GetOwner());
    if (ownerPawn == nullptr)
    {
        return nullptr;
    }

    return ownerPawn->GetController();
}

bool AGun::GetGunTrace(FHitResult& Hit, FVector& ShootDirection)
{
    auto ownerController = GetOwnerController();
    if (ownerController == nullptr)
    {
        return false;
    }

    FVector location;
    FRotator rotation;
    ownerController->GetPlayerViewPoint(location, rotation);

    ShootDirection = -rotation.Vector();

    FVector end = location + rotation.Vector() * MaxRange;

    // DrawDebugLine(GetWorld(), location, end, FColor::Red, true, 10.0f, 0, 12.3f);

    // FCollisionObjectQueryParams paramter;
    // bool bSuccess = GetWorld()->LineTraceSingleByObjectType(result, location, end, paramter);

    FCollisionQueryParams params;
    params.AddIgnoredActor(this);
    params.AddIgnoredActor(GetOwner());

    return GetWorld()->LineTraceSingleByChannel(Hit, location, end, ECollisionChannel::ECC_GameTraceChannel1, params);
}
