// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "kismet/GameplayStatics.h"

void AShooterAIController::BeginPlay()
{
    Super::BeginPlay();
}

void AShooterAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    auto playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (playerPawn == nullptr)
    {
        return;
    }

    auto hasSight = LineOfSightTo(playerPawn);
    if (hasSight)
    {
        SetFocus(playerPawn);
        MoveToActor(playerPawn, AcceptanceRadius);
    }
    else
    {
        ClearFocus(EAIFocusPriority::Gameplay);
        StopMovement();
    }
}
