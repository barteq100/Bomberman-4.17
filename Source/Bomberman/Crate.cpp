// Fill out your copyright notice in the Description page of Project Settings.

#include "Crate.h"
#include "Classes/Components/DestructibleComponent.h"
#include "Door.h"
#include "Pickup.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

int ACrate::NumberOfCrates = 0;
bool ACrate::isDoorSpawned = false;
// Sets default values
ACrate::ACrate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	CrateBox = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Crate"));
	GetBox()->SetEnableGravity(true);
	RootComponent = CrateBox;
	NumberOfCrates++;
	
}

// Called when the game starts or when spawned
void ACrate::BeginPlay()
{
	Super::BeginPlay();
	
}

bool ACrate::ShouldSpawnDoor()
{
	//return (FMath::Rand() % (NumberOfCrates-1) == 1) ? true : false;
	return true;
}

bool ACrate::ShouldSpawnPickup()
{
	return (FMath::Rand() % 4 == 1) ? true : false;
}

void ACrate::SpawnPickup()
{
	if (ShouldSpawnPickup()) {
		int i = FMath::Rand() % Pickups.Num();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = Instigator;

		GetWorld()->SpawnActor<APickup>(Pickups[i], GetActorLocation(), GetActorRotation(), SpawnParams);
	}
}

// Called every frame
void ACrate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ACrate::OnExploded()
{
	if (!ADoor::Spawned) {
		bool spawn = ShouldSpawnDoor();
		if (spawn) {
			ADoor::Spawned = true;

			FActorSpawnParameters SpawnParams;
			SpawnParams.Instigator = Instigator;

			GetWorld()->SpawnActor<ADoor>(Door, GetActorLocation(), GetActorRotation(), SpawnParams);
		}
	}
	else {
		SpawnPickup();
	}
	NumberOfCrates--;
	Destroy();
}
