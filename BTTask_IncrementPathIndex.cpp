// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_NPC/AI_Tasks/BTTask_IncrementPathIndex.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <AI_NPC/NPC_AIController.h>
#include <AI_NPC/NPC.h>

UBTTask_IncrementPathIndex::UBTTask_IncrementPathIndex(FObjectInitializer const& ObjectInitializer) :
	UBTTask_BlackboardBase{ObjectInitializer}
{
	NodeName = TEXT("Increment Path Index");
}

EBTNodeResult::Type UBTTask_IncrementPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// try and get the AI controller
	if (auto* const Cont = Cast<ANPC_AIController>(OwnerComp.GetAIOwner()))
	{
		// try and get the NPC
		if (auto* const NPC = Cast<ANPC>(Cont->GetPawn()))
		{
			// try and get the blackboard
			if (auto* const BC = OwnerComp.GetBlackboardComponent())
			{
				// get number of patrol points and min and max indices
				auto const NoOfPoints = NPC->GetPatrolPath()->Num();
				auto const MinIndex = 0;
				auto const MaxIndex = NoOfPoints - 1;
				auto Index = BC->GetValueAsInt(GetSelectedBlackboardKey());

				// change direction if we are at the first or last index if we are in bidirectional mode
				if (bBiDirectional)
				{
					if (Index >= MaxIndex && Direction == EDirectionType::Forward)
					{
						Direction = EDirectionType::Reverse;
					}
					else if (Index == MinIndex && Direction == EDirectionType::Reverse)
					{
						Direction = EDirectionType::Forward;
					}
				}

				// set new value of index to blackboard
				BC->SetValueAsInt(GetSelectedBlackboardKey(),
					(Direction == EDirectionType::Forward ? ++Index : --Index) % NoOfPoints);

				// finish with success
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}

	// something went wrong so fail
	return EBTNodeResult::Failed;
}
