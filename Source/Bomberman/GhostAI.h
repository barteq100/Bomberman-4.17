// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GhostAI.generated.h"

/**
 * 
 */
UCLASS()
class BOMBERMAN_API AGhostAI : public AAIController
{
	GENERATED_BODY()

	UPROPERTY(transient)
		class UBlackboardComponent* BlackboardComp;

	UPROPERTY(transient)
		class UBehaviorTreeComponent* BehaviorTreeComp;

public:

	AGhostAI();
	
	virtual void Possess(APawn* InPawn) override;

	uint8 GhostKeyId;
	
};
