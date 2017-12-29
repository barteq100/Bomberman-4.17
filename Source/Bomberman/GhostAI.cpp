// Fill out your copyright notice in the Description page of Project Settings.

#include "GhostAI.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Ghost.h"




AGhostAI::AGhostAI()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));

	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
}

void AGhostAI::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	AGhost* Ghost = Cast<AGhost>(InPawn);

	if(Ghost && Ghost->GhostBehavior)
	{
		BlackboardComp->InitializeBlackboard(*Ghost->GhostBehavior->BlackboardAsset);
		GhostKeyId = BlackboardComp->GetKeyID("Target");

		BehaviorTreeComp->StartTree(*Ghost->GhostBehavior);
	}
}
