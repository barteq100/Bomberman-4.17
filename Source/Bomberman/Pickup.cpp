// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickup.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "BombermanCharacter.h"

TArray<TWeakObjectPtr<APickup>> APickup::pickups;
// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup"));
	TScriptDelegate<FWeakObjectPtr> onHitFunc;
	onHitFunc.BindUFunction(this, "OnPickup");
	GetMesh()->OnComponentBeginOverlap.Add(onHitFunc);
	GetMesh()->bGenerateOverlapEvents = true;
	SetRootComponent(PickupMesh);
	
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	APickup::pickups.Add(this);
}

void APickup::OnPickup(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	ABombermanCharacter* const character = Cast<ABombermanCharacter>(OtherActor);
	if (character) {
		GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Yellow, FString("You picked a pickup"));
		Action();
		Destroy();
	}
}

void APickup::Action()
{
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

