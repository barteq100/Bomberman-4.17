// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_CheckForPlayer.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "BombermanCharacter.h"
#include "GhostAI.h"
#include "Ghost.h"


UBTService_CheckForPlayer::UBTService_CheckForPlayer()
{
	bCreateNodeInstance = true;
}

void UBTService_CheckForPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AGhostAI* GhostPC = Cast<AGhostAI>(OwnerComp.GetAIOwner());

	if(GhostPC)
	{
		ABombermanCharacter *Character = Cast<ABombermanCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if(Character)
		{
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(GhostPC->GhostKeyId, Character);
		}
	}
}
