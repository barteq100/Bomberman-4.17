// Fill out your copyright notice in the Description page of Project Settings.

#include "Bomb.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Classes/Particles/ParticleSystemComponent.h"
#include "Explosion.h"
#include "PhysicsEngine/DestructibleActor.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "MapCreator.h"

int ABomb::Power  = 1;
int ABomb::NumberOfBombs = 0;
// Sets default values
ABomb::ABomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BombMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PillarMesh"));
	SetRootComponent(BombMesh);
	GetMesh()->SetSimulatePhysics(true);
	Delay = 2.0f;
	exploded = false;
	
}

void ABomb::setMapX(int X)
{
	MapX = X;
}

void ABomb::setMapY(int Y)
{
	MapY = Y;
}

int ABomb::getMapX()
{
	return MapX;
}

int ABomb::getMapY()
{
	return MapY;
}

void ABomb::setPosition(FVector newPosition)
{
	Position = newPosition;
}

void ABomb::PowerUpBomb()
{
	Power++;
}

// Called when the game starts or when spawned
void ABomb::BeginPlay()
{
	Super::BeginPlay();
	NumberOfBombs++;
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &ABomb::OnExplosion, Delay, false);

}

// Called every frame
void ABomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABomb::Explode() {

	MapX = AMapCreator::WorldXToMapX(GetActorLocation().X);
	MapY = AMapCreator::WorldYToMapY(GetActorLocation().Y);

	UWorld* const World = GetWorld();
	FVector tvec = GetActorLocation();

	SpawnExplosion(tvec);
	float distance = AMapCreator::PillarHeight;


	for (int i = 1; i < Power + 1; i++) {
		SpawnExplosion(FVector(tvec.X + (distance * i), tvec.Y, tvec.Z));
		SpawnExplosion(FVector(tvec.X - (distance * i), tvec.Y, tvec.Z));
		SpawnExplosion(FVector(tvec.X, tvec.Y + (distance * i), tvec.Z));
		SpawnExplosion(FVector(tvec.X, tvec.Y - (distance * i), tvec.Z));
	}
}

void ABomb::OnExplosion() {
		Explode();
		GetWorldTimerManager().ClearTimer(SpawnTimer);
		NumberOfBombs--;
		Destroy();
}

void ABomb::SpawnExplosion(FVector position) {
	FRotator SpawnRotation;
	SpawnRotation.Yaw = 0.0f;
	SpawnRotation.Pitch = 0.0f;
	SpawnRotation.Roll = 0.0f;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;

	AExplosion* const SpawnedExplosion = GetWorld()->SpawnActor<AExplosion>(Explosion, position, SpawnRotation, SpawnParams);
	Explosions.Add(SpawnedExplosion);
}

void ABomb::TryToDestroyCrate(int x, int y, FTransform texp) {

}

void ABomb::Destroy()
{
	for (int32 iExplosion = 0; iExplosion < Explosions.Num(); ++iExplosion) {
		GetWorld()->DestroyActor(Explosions[iExplosion]);
	}
	Super::Destroy();
}
	



