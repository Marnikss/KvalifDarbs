// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_NPC/AI_Tasks/BTTask_MeleeAttack.h"
#include "AI_NPC/CombatInterface.h"
#include "AIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/LatentActionManager.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"



UBTTask_MeleeAttack::UBTTask_MeleeAttack()
{
	NodeName = TEXT("Melee Attack");
}






























































EBTNodeResult::Type UBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// if we are out of range, do not attack the player
	auto const OutOfRange = !OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());
	if (OutOfRange)
	{
		UE_LOG(LogTemp, Display, TEXT("Executing OUTOFRANGE"));
		// finish the task
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	// we are in range so get the AI's controller and the NPC itself
	auto const * const cont = OwnerComp.GetAIOwner();
	auto* const npc = Cast<ANPC>(cont->GetPawn());

	// if the NPC supports the ICombatInterface, cast and call the Execute_MeleeAttack function
	if (auto* const icombat = Cast<ICombatInterface>(npc))
	{
		UE_LOG(LogTemp, Display, TEXT("Executing MeleeAttack"));
		// necessary check to see if the montage has finished so we dont try and play it again
		if (MontageHasFinished(npc))
		{
			icombat->MeleeAttack_Implementation();
		}
	}
	// finish the task
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Type();
}









bool UBTTask_MeleeAttack::MontageHasFinished(ANPC * const npc)
{
	UE_LOG(LogTemp, Warning, TEXT("MontageHasFinished function called"));
	return npc->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(npc->GetMontage());
}
