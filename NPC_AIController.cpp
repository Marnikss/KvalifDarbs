// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_NPC/NPC_AIController.h"
#include <AI_NPC/NPC.h>
#include <Perception/AIPerceptionTypes.h>
#include <Perception/AIPerceptionComponent.h>
#include <Perception/AISenseConfig_Sight.h>
#include <Speele/Cilveks.h>
#include "BehaviorTree/BlackboardComponent.h"




ANPC_AIController::ANPC_AIController(FObjectInitializer const& ObjectInitializer)
{
	SetupPerceptionSystem();
}

void ANPC_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (ANPC* const npc = Cast<ANPC>(InPawn))
	{
		if (UBehaviorTree* const tree = npc->GetBehaviorTree())
		{
			UBlackboardComponent* b;
			UseBlackboard(tree->BlackboardAsset, b);
			Blackboard = b;
			RunBehaviorTree(tree);
		}
	}
}

void ANPC_AIController::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(
		TEXT("Perception Component")));
		SightConfig->SightRadius = 1000.f;//500
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.f;
		SightConfig->PeripheralVisionAngleDegrees = 120.f;//90
		SightConfig->SetMaxAge(5.f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 600.f;//520
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->SetDominantSense(*SightConfig->
		GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this,
		&ANPC_AIController::OnTargetDetected);
		GetPerceptionComponent()->ConfigureSense((*SightConfig));
	}
}

void ANPC_AIController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (auto* const ch = Cast<ACilveks>(Actor))
	{
		// Print a message indicating that OnTargetDetected is called
		UE_LOG(LogTemp, Warning, TEXT("OnTargetDetected called!"));

		GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());

		// Print the value of CanSeePlayer after updating the blackboard
		bool bCanSeePlayer = Stimulus.WasSuccessfullySensed();
		UE_LOG(LogTemp, Warning, TEXT("CanSeePlayer: %s"), bCanSeePlayer ? TEXT("true") : TEXT("false"));

	}
}
