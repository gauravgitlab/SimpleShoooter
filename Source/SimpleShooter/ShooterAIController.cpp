// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

// Called when the game starts or when spawned
void AShooterAIController::BeginPlay()
{
    Super::BeginPlay();

    // get player from the world, using `GetPlayerPawn`
    PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    if(AIBehavior != nullptr)
    {
        // run the behavior tree
        RunBehaviorTree(AIBehavior);

        // set the variables values, created in Behavior tree
        
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
    }
}

void AShooterAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // if(PlayerPawn == nullptr)
    //     return;

    // // if line of sight, move towards player by setting PlayerLocation and Last known location by chasing...
    // // else clear the player Location, so AI can move towards the last known location of player to investigate
    // if(LineOfSightTo(PlayerPawn))
    // {
    //     // set the variables values, created in Behavior tree
    //     GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
    //     GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), PlayerPawn->GetActorLocation());
    // }
    // else
    // {
    //     GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
    // }
}

bool AShooterAIController::IsDead() const
{
    AShooterCharacter* controlledCharacter = Cast<AShooterCharacter>(GetPawn());
    if(controlledCharacter != nullptr)
        return controlledCharacter->IsDead();

    return true;
}

// old code to move towards the player, we are replacing this with Behavior tree

// if line of sight, move towards player and set focus
// else clear focus and stop movement
//     if(LineOfSightTo(PlayerPawn))
//     {
//         // set focus of AI to player Pawn, it soes not require Nav Mesh for level
//         // its just rotating towards the player
//         SetFocus(PlayerPawn);

//         // move towards Player, we already have NavMesh in the gameplay level
//         MoveToActor(PlayerPawn, AcceptanceRadius);
//     }
//     else
//     {
//         ClearFocus(EAIFocusPriority::Gameplay);
//         StopMovement();
//     }
