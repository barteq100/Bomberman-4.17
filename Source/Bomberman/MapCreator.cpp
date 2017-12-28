// Fill out your copyright notice in the Description page of Project Settings.

#include "MapCreator.h"
#include "Pillar.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "Public/Math/BoxSphereBounds.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "BombermanCharacter.h"

std::vector<std::vector<TWeakObjectPtr<ACrate>>> AMapCreator::BomberVector;
FVector AMapCreator::SpawnOrigin;
float AMapCreator::PillarHeight;
int AMapCreator::HowManyHorizontaly;
int AMapCreator::HowManyVertically; 
AMapCreator* AMapCreator::mapCreatorInstance = nullptr;

// Sets default values
AMapCreator::AMapCreator()
{

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MapBox"));
	RootComponent = MapBox;
	

}


// Called when the game starts or when spawned
void AMapCreator::BeginPlay()
{
	Super::BeginPlay();
	aCratePtr = *aCrate;
	MapWidth = MapBox->Bounds.BoxExtent.Y * 2;
	MapHeight = MapBox->Bounds.BoxExtent.X * 2;
	if (AMapCreator::mapCreatorInstance == nullptr)
	{
		AMapCreator::mapCreatorInstance = this;
	}
	APillar* const SpawnedPillar = SpawnPillar();
	if (SpawnedPillar != NULL) {
		PillarHeight = SpawnedPillar->GetPillarMesh()->Bounds.BoxExtent.Y * 2;
		PillarWidth = SpawnedPillar->GetPillarMesh()->Bounds.BoxExtent.X * 2;
		HowManyHorizontaly = static_cast<int>(MapWidth / PillarHeight);
		HowManyVertically = static_cast<int>(MapHeight / PillarHeight);
		BomberVector.resize(HowManyVertically, std::vector<TWeakObjectPtr<ACrate>>(HowManyHorizontaly));
		TWeakObjectPtr<ACrate> TestCrate = SpawnCrate(FVector(0.0f, 0.0f, 0.0f));
		CrateZ = TestCrate->GetBox()->Bounds.BoxExtent.Z;
		TestCrate->Destroy();
		GetWorld()->DestroyActor(SpawnedPillar);
	}
	SpawnPillarsOnMap();
	SpawnCratesOnMap();
	MakeSpaceForCharacter();
}

// Called every frame
void AMapCreator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

APillar* const AMapCreator::SpawnPillar() {
	FVector SpawnLocation;
	SpawnLocation.X = 0.0f;
	SpawnLocation.Y = 0.0f;
	SpawnLocation.Z = 0.0f;

	FRotator SpawnRotation;
	SpawnRotation.Yaw = 0.0f;
	SpawnRotation.Pitch = 0.0f;
	SpawnRotation.Roll = 0.0f;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;

	if (aPillar != NULL) {
		UWorld* const World = GetWorld();
		if (World != NULL) {
			APillar* const SpawnedPillar = GetWorld()->SpawnActor<APillar>(aPillar, SpawnLocation, SpawnRotation, SpawnParams);
			return SpawnedPillar;
		}
		return NULL;
	}
	return NULL;
}

void AMapCreator::SpawnPillarsOnMap() {

	FVector SpawnLocation;
	SpawnLocation.X = 0.0f + PillarHeight;
	SpawnLocation.Y = MapWidth /2  - PillarHeight;
	SpawnLocation.Z = 0.0f;
	SpawnOrigin = SpawnLocation;

	FRotator SpawnRotation;
	SpawnRotation.Yaw = 0.0f;
	SpawnRotation.Pitch = 0.0f;
	SpawnRotation.Roll = 0.0f;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;

	if (aPillar != NULL) {
		UWorld* const World = GetWorld();
		if (World != NULL) {
			int j = 0;
			int i = 0;
			for (j = 0; j < HowManyHorizontaly; j++) {
				if (j % 2 == 0) {
					for (i = 0; i < HowManyVertically; i++) {
						if (i % 2 == 0) {
							APillar* const SpawnedPillar = World->SpawnActor<APillar>(aPillar, SpawnLocation, SpawnRotation, SpawnParams);
							//BomberVector[i][j] = SpawnedPillar;
							SpawnLocation.X += PillarHeight * 2;
						}
					}
				}
					SpawnLocation.X = 0.0f + PillarHeight;
					SpawnLocation.Y -= PillarHeight;	
			}	
		}	
	}
}

FVector AMapCreator::MapToWorld(int x, int y) {
	FVector mapToWorld;
	mapToWorld.X = SpawnOrigin.X + (x * PillarHeight);
	mapToWorld.Y = SpawnOrigin.Y - (y * PillarHeight);
	mapToWorld.Z = 0.0f;

	return mapToWorld;
}

int AMapCreator::WorldXToMapX(float x) {
	int newX = FMath::Abs<float>((x  - SpawnOrigin.X ) / PillarHeight);
	if (newX <= HowManyVertically) return newX;
	else return HowManyVertically;
}

int AMapCreator::WorldYToMapY(float y) {

	int newY = FMath::Abs<float>((y - SpawnOrigin.Y ) / PillarHeight);
	if (newY <= HowManyHorizontaly) return newY;
	else return HowManyHorizontaly;
}

bool AMapCreator::ShouldSpawnCrate() {
	return (FMath::Rand() % 3 == 1 )? true : false;
}

TWeakObjectPtr<ACrate> AMapCreator::SpawnCrate(FVector SpawnLocation) {
	SpawnLocation.Z = CrateZ;
	FRotator SpawnRotation;
	SpawnRotation.Yaw = 0.0f;
	SpawnRotation.Pitch = 0.0f;
	SpawnRotation.Roll = 0.0f;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
		UWorld* const World = GetWorld();
		if (World != nullptr && aCrate != NULL)
		{
			TWeakObjectPtr<ACrate> SpawnedCrate = World->SpawnActor<ACrate>(aCrate, SpawnLocation, SpawnRotation, SpawnParams);
			return SpawnedCrate;
		}
		return nullptr;
}

void AMapCreator::SpawnCratesOnMap() {
	for (int j = 0; j < HowManyHorizontaly; j++) {
		for (int i = 0; i < HowManyVertically; i++) {
			if (j % 2 == 0) {
				if (i % 2 == 1) {
					if (ShouldSpawnCrate()) {
						try {
							BomberVector[i][j] = SpawnCrate(MapToWorld(i, j));
						}
						catch (...) {

						}
					}								
				}
			}
			else {
				if (ShouldSpawnCrate()) {
					try {
						BomberVector[i][j] = SpawnCrate(MapToWorld(i, j));
					}
					catch (...) {

					}
				}				
			}
		}
	}
}

void AMapCreator::MakeSpaceForCharacter() {
	for (int i = 0; i < 3; i++) {
		if (BomberVector[i][3] != nullptr) {
			BomberVector[i][3]->Destroy();
		}
	}
	
	if (BomberVector[1][4] != nullptr) {
		BomberVector[1][4]->Destroy();
	}

	if (BomberVector[1][2] != nullptr) {
		BomberVector[1][2]->Destroy();
	}

}

void AMapCreator::ClearMap() {
	for (int j = 0; j < HowManyHorizontaly; j++) {
		for (int i = 0; i < HowManyVertically; i++) {
			if (BomberVector[i][j] != nullptr) {
				try {
					BomberVector[i][j]->Destroy();
				}
				catch (...) {
					BomberVector[i][j] = nullptr;
				}
			}
		}
	}
	for (auto p : APickup::GetAllPickups()) {
		p->Destroy();
	}
	BomberVector.clear();
	BomberVector.resize(HowManyVertically, std::vector<TWeakObjectPtr<ACrate>>(HowManyHorizontaly));
	
}

void AMapCreator::RestartMap() {
	ClearMap();
	ABombermanCharacter::GetCharacter()->SetActorLocation(ABombermanCharacter::startPosition);
	if (aCrate != NULL) {
		SpawnCratesOnMap();
		MakeSpaceForCharacter();
		ABombermanCharacter::GetCharacter()->SetActorLocationAndRotation(FVector(350.f,0.f,150.f), FRotator(ABombermanCharacter::actorRotation).Quaternion());
	}
}
