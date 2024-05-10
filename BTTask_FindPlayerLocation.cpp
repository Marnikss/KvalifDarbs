// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_NPC/AI_Tasks/BTTask_FindPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include <Kismet/GameplayStatics.h>
#include <NavigationSystem.h>


UBTTask_FindPlayerLocation::UBTTask_FindPlayerLocation(FObjectInitializer const& ObjectInitializer) :
	UBTTask_BlackboardBase{ ObjectInitializer }
{
	NodeName = TEXT("Find Player Location");
}

EBTNodeResult::Type UBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get player character 
	if (auto* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		//get player location to use an origin
		auto const PlayerLocation = Player->GetActorLocation();
		if (SearchRandom)
		{
			FNavLocation Loc;

			// get the navigation system and generate a random location near the player
			if (auto* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				// try to get a random location near the player
				if (NavSys->GetRandomPointInNavigableRadius(PlayerLocation, SearchRadius, Loc))
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Loc.Location);
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
			}
		}
		else 
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerLocation);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
