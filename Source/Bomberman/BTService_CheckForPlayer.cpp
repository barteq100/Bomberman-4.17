// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_CheckForPlayer.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Kismet/KismetMathLibrary.h"
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
	AGhost* Ghost = Cast<AGhost>(GhostPC->GetPawn());
	if(GhostPC)
	{
		ABombermanCharacter *Character = Cast<ABombermanCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if(Character)
		{
			float distance = Character->GetDistanceTo(Ghost);
			if ( distance < DistanceToSee) {
				auto rotation = UKismetMathLibrary::FindLookAtRotation(Ghost->GetActorLocation(), Character->GetActorLocation());
				float angle = rotation.Pitch;
				if( angle <= 45)
				OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(GhostPC->GhostKeyId, Character);
			}
			else
			{
				OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(GhostPC->GhostKeyId, nullptr);
			}
		}
	}
}
