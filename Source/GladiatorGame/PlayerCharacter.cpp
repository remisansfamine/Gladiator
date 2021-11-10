// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "LifeComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

APlayerCharacter::APlayerCharacter() 
	: AGladiatorGameCharacter()
{
	ActivateCamera();

	lifeComponent->SetLife(5);
	lifeComponent->invicibleCooldown = 1.f;
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("LockOn", IE_Pressed, this, &APlayerCharacter::SetCameraLock);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::Attack);

	PlayerInputComponent->BindAction<FDefend>("Defend", IE_Pressed, this, &APlayerCharacter::Defend, true);
	PlayerInputComponent->BindAction<FDefend>("Defend", IE_Released, this, &APlayerCharacter::Defend, false);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CameraLock();
}

void APlayerCharacter::CameraLock()
{
	if (cameraLockTarget)
		LookAtTarget(cameraLockTarget, lockOnSpeed);
}

void APlayerCharacter::SetCameraLock()
{
	isLocking ? SetCameraLockOff() : SetCameraLockOn();
}

void APlayerCharacter::SetCameraLockOn()
{
	isLocking = true;

	cameraLockTarget = GetOtherGladiator(minLockOnDistance, maxLockOnDistance);

	//CameraBoom->bUsePawnControlRotation = false; // Rotate the arm based on the controller

	if (!cameraLockTarget)
		SetCameraLockOff();
}

void APlayerCharacter::SetCameraLockOff()
{
	isLocking = false;

	//CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	cameraLockTarget = nullptr;
}