// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "BombermanCharacter.h"
#include "MapCreator.h"
#include "Bomb.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"

//////////////////////////////////////////////////////////////////////////
// ABombermanCharacter
FName ABombermanCharacter::fname;
FVector ABombermanCharacter::startPosition;
FRotator ABombermanCharacter::actorRotation;
TWeakObjectPtr<ABombermanCharacter> ABombermanCharacter::character;
int ABombermanCharacter::MaxNumberOfBombs = 2;
ABombermanCharacter::ABombermanCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	GetMesh()->bGenerateOverlapEvents = true;
	fname = GetFName();
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	startPosition = GetActorLocation();
	actorRotation = GetActorRotation();
	ABombermanCharacter::character = this;
	
}

//////////////////////////////////////////////////////////////////////////
// Input

void ABombermanCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("SpawnBomb", IE_Released, this, &ABombermanCharacter::SpawnBomb);
	

	PlayerInputComponent->BindAxis("MoveForward", this, &ABombermanCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABombermanCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ABombermanCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ABombermanCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ABombermanCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ABombermanCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ABombermanCharacter::OnResetVR);
}


void ABombermanCharacter::OnExploded()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString("You Died"));
	Destroy();
}


void ABombermanCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ABombermanCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ABombermanCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ABombermanCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ABombermanCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ABombermanCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ABombermanCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ABombermanCharacter::SpawnBomb() {
	if (ABomb::NumberOfBombs + 1 > ABombermanCharacter::MaxNumberOfBombs) return;
	FVector SpawnLocation = GetActorLocation();
	SpawnLocation += (GetActorForwardVector() * AMapCreator::PillarHeight);

	
	FRotator SpawnRotation;
	SpawnRotation.Yaw = 0.0f;
	SpawnRotation.Pitch = 0.0f;
	SpawnRotation.Roll = 0.0f;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Cast<ACharacter>(this);
	SpawnParams.Instigator = Instigator;

	ABomb* const SpawnedBomb  = GetWorld()->SpawnActor<ABomb>(aBomb, SpawnLocation, SpawnRotation, SpawnParams);
	SpawnedBomb->setMapX(AMapCreator::WorldXToMapX(SpawnLocation.X));
	SpawnedBomb->setMapY(AMapCreator::WorldYToMapY(SpawnLocation.Y));
	SpawnedBomb->setPosition(SpawnLocation);
	
}

//void ABombermanCharacter::RestartPosition()
//{
//	GetWorld()->GetFirstPlayerController()->SetActorLocationAndRotation(ABombermanCharacter::startPosition, FRotator(ABombermanCharacter::actorRotation).Quaternion());
//}

//void ABombermanCharacter::HideCharacter() {
//
//	if (ABombermanCharacter::character == nullptr) return;
//	ABombermanCharacter::character->SetActorLocation(FVector(ABombermanCharacter::startPosition.X, ABombermanCharacter::character->startPosition.Y, ABombermanCharacter::character->startPosition.Z + 5000.f));
//}
//
//void ABombermanCharacter::ShowCharacter() {
//
//	if (ABombermanCharacter::character == nullptr) return;
//	ABombermanCharacter::character->SetActorHiddenInGame(false);
//}
