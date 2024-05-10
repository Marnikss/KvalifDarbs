// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_NPC/BTService_IsPlayerInMeleeRange.h"
#include "AI_NPC/NPC_AIController.h"
#include "AI_NPC/NPC.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTService_IsPlayerInMeleeRange::UBTService_IsPlayerInMeleeRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player In Melee Range");
}

void UBTService_IsPlayerInMeleeRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get controller and npc
	auto const * const cont = Cast<ANPC_AIController>(OwnerComp.GetAIOwner());
	auto const * const npc = Cast<ANPC>(cont->GetPawn());

	// get player character
	auto const * const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	// write true or false to the blackboard key depending on whether or not the player is in range
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(
		GetSelectedBlackboardKey(),
		npc->GetDistanceTo(player) <= MeleeRange
	);
}
