// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_NPC/AttackAnimNotifyState.h"
#include <AI_NPC/NPC.h>
#include <Speele/Cilveks.h>

/*
void UAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("NotifyBegin"));
	UE_LOG(LogTemp, Warning, TEXT("NotifyBegin"));
	if (MeshComp && MeshComp->GetOwner())
	{
		if (ACilveksBase* const character = Cast<ACilveksBase>(MeshComp->GetOwner()))
		{
			character->AttackStart();
		}
	}
}

void UAttackAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	
	if (MeshComp && MeshComp->GetOwner())
	{
		if (ACilveksBase* const character = Cast<ACilveksBase>(MeshComp->GetOwner()))
		{
			character->AttackEnd();
		}
	}
	
}
*/