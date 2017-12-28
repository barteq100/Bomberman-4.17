// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pillar.h"
#include "Classes/Components/BoxComponent.h"
#include "vector"
#include "Crate.h"
#include "GameFramework/Actor.h"
#include "MapCreator.generated.h"

UCLASS()
class BOMBERMAN_API AMapCreator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapCreator();
	int NumberOfCrates;

	// X is longer
	static FVector MapToWorld(int x, int y);
	static int WorldXToMapX(float x);
	static int WorldYToMapY(float y);
	static std::vector<std::vector<TWeakObjectPtr<ACrate>>> BomberVector;
	static FVector SpawnOrigin;
	static float PillarHeight;
	static int HowManyHorizontaly;
	static int HowManyVertically;

	static AMapCreator* getInstance() { return mapCreatorInstance; }
	void RestartMap();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "MapCreation")
	TSubclassOf<class APillar> aPillar;

	UPROPERTY(EditAnywhere, Category = "MapCreation", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ACrate> aCrate;

	
private:
	static AMapCreator* mapCreatorInstance;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MapCreation", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* MapBox;

	float MapWidth;
	float MapHeight;
	float PillarWidth;
	
	float CrateZ = 0.0f;

	UClass* aCratePtr;
	
	APillar* const SpawnPillar();
	bool ShouldSpawnCrate();
	TWeakObjectPtr<ACrate> SpawnCrate(FVector SpawnLocation);


	void SpawnPillarsOnMap();
	void SpawnCratesOnMap();
	void MakeSpaceForCharacter();
	void ClearMap();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
