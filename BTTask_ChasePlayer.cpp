// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_NPC/AI_Tasks/BTTask_ChasePlayer.h"
#include <AI_NPC/NPC_AIController.h>
#include "BehaviorTree/BlackboardComponent.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>

UBTTask_ChasePlayer::UBTTask_ChasePlayer(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	// ja NPC tiek kontrolēts ar AIController
	if (auto* const cont = Cast<ANPC_AIController>(OwnerComp.GetAIOwner()))
	{
		// iegūst spēlētāja atrašanās vietu no blackboard komponenta/mainīgā
		auto const PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());

		// kustēties uz spēlētāja atrašanās vietu
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(cont, PlayerLocation);

		// atgriež, ka sanācis uzdotais
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	// ja nesanāca atgriež, ka nav sanācis
	return EBTNodeResult::Failed;
}
