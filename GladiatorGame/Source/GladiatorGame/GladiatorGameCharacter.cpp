// Copyright Epic Games, Inc. All Rights Reserved.

#include "GladiatorGameCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "LifeComponent.h"


AGladiatorGameCharacter::AGladiatorGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 250.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	DeactivateCamera();

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	lifeComponent = CreateDefaultSubobject<ULifeComponent>("LifeComponent");
	lifeComponent->OnKill.AddDynamic(this, &AGladiatorGameCharacter::OnDeath);

	canMove = true;
}

void AGladiatorGameCharacter::ActivateCamera() 
{ 
	CameraBoom->Activate(); 
}

void AGladiatorGameCharacter::DeactivateCamera() 
{ 
	CameraBoom->Deactivate(); 
}

void AGladiatorGameCharacter::OnDeath()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message!"));
	canMove = false;
}

void AGladiatorGameCharacter::MoveForward(float Value)
{
	Move(EAxis::X, Value);
}

void AGladiatorGameCharacter::MoveRight(float Value)
{
	Move(EAxis::Y, Value);
}

void AGladiatorGameCharacter::Move(EAxis::Type axis, float value)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::FromInt(canMove));

	if (!canMove || !Controller || value == 0.0f )
		return;

	// find out which way is right
	const FRotator rot = Controller->GetControlRotation();
	const FRotator yawRot(0, rot.Yaw, 0);

	// get right vector and add movement in that direction
	const FVector dir = FRotationMatrix(yawRot).GetUnitAxis(axis);
	Move(dir, value);
}

void AGladiatorGameCharacter::Move(const FVector& direction, float value)
{
	if (value == 0.0f)
		return;

	AddMovementInput(direction, value);
}