// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "ShooterAIController.h"

void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled)
{
    Super::PawnKilled(PawnKilled);

    auto playerController = Cast<APlayerController>(PawnKilled->GetController());
    if (playerController != nullptr)
    {
        EndGame(false);
    }

    for (auto controller : TActorRange<AShooterAIController>(GetWorld()))
    {
        if (!controller->IsDead())
        {
            return;
        }
    }
    EndGame(true);
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner)
{
    for (auto controller : TActorRange<AController>(GetWorld()))
    {
        auto isWinner = controller->IsPlayerController() == bIsPlayerWinner;
        controller->GameHasEnded(controller->GetPawn(), isWinner);
    }
}
