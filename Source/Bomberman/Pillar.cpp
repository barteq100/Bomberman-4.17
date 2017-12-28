// Fill out your copyright notice in the Description page of Project Settings.

#include "Pillar.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "Public/Math/BoxSphereBounds.h"


// Sets default values
APillar::APillar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PillarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PillarMesh"));
	RootComponent = PillarMesh;
	
}

// Called when the game starts or when spawned
void APillar::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APillar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APillar::OnExploded()
{
}

