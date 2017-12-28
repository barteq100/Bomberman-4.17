// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IExplodable.h"
#include "GameFramework/Actor.h"
#include "Pillar.generated.h"

UCLASS()
class BOMBERMAN_API APillar : public AActor, public IIExplodable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APillar();

	FORCEINLINE class UStaticMeshComponent* GetPillarMesh() const { return PillarMesh; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pillar", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PillarMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnExploded() override;
	
};
