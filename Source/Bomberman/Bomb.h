// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Explosion.h"
#include "Bomb.generated.h"

UCLASS()
class BOMBERMAN_API ABomb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABomb();
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return BombMesh; }

	void setMapX(int X);
	void setMapY(int Y);
	int getMapX();
	int getMapY();

	void setPosition(FVector newPosition);
	void Explode();
	float Delay;
	FTimerHandle SpawnTimer;

	static int Power;

	static void PowerUpBomb();
	static int NumberOfBombs;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bomb", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BombMesh;


	UPROPERTY(EditAnywhere, Category = "Bomb")
		TSubclassOf<class AExplosion> Explosion;

	
	int MapX;
	int MapY;
	FVector Position;
	
	
	bool exploded;

	void OnExplosion();

	void SpawnExplosion(FVector position);

	void TryToDestroyCrate(int x, int y, FTransform texp);

	TArray<AExplosion*> Explosions;
	
	virtual void Destroy();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	
	
};
