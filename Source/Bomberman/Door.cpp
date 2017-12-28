// Fill out your copyright notice in the Description page of Project Settings.

#include "Door.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "BombermanCharacter.h"
#include "MapCreator.h"


bool ADoor::Spawned = false;
// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DOOR"));
	TScriptDelegate<FWeakObjectPtr> onHitFunc;
	onHitFunc.BindUFunction(this, "OnEnter");
	GetMesh()->OnComponentBeginOverlap.Add(onHitFunc);
	GetMesh()->bGenerateOverlapEvents = true;
	SetRootComponent(DoorMesh);
	


}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoor::OnEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor->GetFName().IsEqual(ABombermanCharacter::fname)) {
		ADoor::Spawned = false;
		GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Yellow ,FString("You win!"));
		AMapCreator::getInstance()->RestartMap();
		Destroy();
	}
}

