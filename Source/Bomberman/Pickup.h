// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class BOMBERMAN_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	static TArray<TWeakObjectPtr<APickup>>& GetAllPickups() { return pickups; }

	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return PickupMesh;}

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PickupMesh;

	UFUNCTION()
	virtual void OnPickup(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void Action();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	static TArray<TWeakObjectPtr<APickup>> pickups;
	
	
};
