// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"


// Called when the game starts or when spawned
void AShooterPlayerController::BeginPlay()
{
    Super::BeginPlay();

    HUD = CreateWidget(this, HUDClass);
    if(HUD != nullptr)
    {
        HUD->AddToViewport();
    }
}

void AShooterPlayerController::GameHasEnded(AActor *EndGameFocus, bool bIsWinner)
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);

    HUD->RemoveFromViewport();

    if(bIsWinner)
    {
        UUserWidget* winScreen = CreateWidget(this, WinScreenClass);
        if(winScreen != nullptr)
        {
            winScreen->AddToViewport();
        }
    }
    else
    {
        UUserWidget* loseScreen = CreateWidget(this, LoseScreenClass);
        if(loseScreen != nullptr)
        {
            loseScreen->AddToViewport();
        }
    }

    GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}
