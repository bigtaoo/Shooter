// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "Blueprint/UserWidget.h"

void AShooterPlayerController::GameHasEnded(class AActor* EndGameFocus/* = nullptr*/, bool bIsWinner/* = false*/)
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);

    auto loseScreen = CreateWidget(this, LoseScreenClass);
    if (loseScreen != nullptr)
    {
        loseScreen->AddToViewport();
    }

    GetWorld()->GetTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}
