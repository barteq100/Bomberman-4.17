// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Explosion.generated.h"

UCLASS()
class BOMBERMAN_API AExplosion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExplosion();
	FORCEINLINE class USphereComponent* GetSphere() const { return ExplosionSphere; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Explosion", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* ExplosionSphere;

	UPROPERTY(EditAnywhere, Category = "Explosion", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* Explosion;

	UFUNCTION()
	virtual void OnExplosion(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SpawnExplosion();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
