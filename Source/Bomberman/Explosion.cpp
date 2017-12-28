// Fill out your copyright notice in the Description page of Project Settings.

#include "Explosion.h"
#include "Classes/Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "IExplodable.h"
#include "MapCreator.h"


// Sets default values
AExplosion::AExplosion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	ExplosionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionSphere"));
	ExplosionSphere->SetBoundsScale(4.0f);
	Explosion = CreateDefaultSubobject<UParticleSystem>(TEXT("Explosion"));
	GetSphere()->InitSphereRadius(AMapCreator::PillarHeight / 2);
	TScriptDelegate<FWeakObjectPtr> onHitFunc;
	onHitFunc.BindUFunction(this, "OnExplosion");
	GetSphere()->OnComponentBeginOverlap.Add(onHitFunc);	
	GetSphere()->OnComponentHit.Add(onHitFunc);
	GetSphere()->bGenerateOverlapEvents = true;
	SetRootComponent(ExplosionSphere);
}

// Called when the game starts or when spawned
void AExplosion::BeginPlay()
{
	Super::BeginPlay();
	SpawnExplosion();
	
}

void AExplosion::OnExplosion(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	TArray<AActor*> OverlappingActors;
	ExplosionSphere->GetOverlappingActors(OverlappingActors);

	for (int32 iOverlapping = 0; iOverlapping < OverlappingActors.Num(); iOverlapping++ ) {
		IIExplodable* const explodable = Cast<IIExplodable>(OverlappingActors[iOverlapping]);
		if(explodable)
		explodable->OnExploded();
	}

}

void AExplosion::SpawnExplosion()
{
	UParticleSystemComponent* expl = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, GetActorTransform());
	expl->SetRelativeScale3D(FVector(4.f));
}

// Called every frame
void AExplosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

