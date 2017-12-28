// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "PowerUPPickup.generated.h"

/**
 * 
 */
UCLASS()
class BOMBERMAN_API APowerUPPickup : public APickup
{
	GENERATED_BODY()

	virtual void Action() override;
		
	
};
