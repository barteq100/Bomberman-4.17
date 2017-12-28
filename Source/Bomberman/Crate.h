// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Pickup.h"
#include "Door.h"
#include "IExplodable.h"
#include "vector"
#include "Classes/Components/DestructibleComponent.h"
#include "GameFramework/Actor.h"
#include "Crate.generated.h"

UCLASS()
class BOMBERMAN_API ACrate : public AActor, public IIExplodable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACrate();

	FORCEINLINE class UDestructibleComponent* GetBox() const { return CrateBox; }

	void DestroyCrate(UParticleSystem& Explosion);
	
	virtual void OnExploded() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	static int NumberOfCrates;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crate", meta = (AllowPrivateAccess = "true"))
	class UDestructibleComponent* CrateBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crate", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class ADoor> Door;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crate", meta = (AllowPrivateAccess = "true"))
		TArray<TSubclassOf<APickup>> Pickups;
	
	bool ShouldSpawnDoor();
	static bool isDoorSpawned;

	bool ShouldSpawnPickup();
	void SpawnPickup();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
