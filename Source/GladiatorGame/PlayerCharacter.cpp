// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "LifeComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GladiatorGameState.h"

APlayerCharacter::APlayerCharacter() 
	: AGladiatorGameCharacter()
{
	ActivateCamera();


}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	healthComponent->SetLife(5);
	healthComponent->invicibleCooldown = 1.f;
	healthComponent->OnKill.AddDynamic(this, &APlayerCharacter::PlayerDeath);
}

void APlayerCharacter::PlayerDeath()
{
	AGladiatorGameState* gameState = Cast<AGladiatorGameState>(GetWorld()->GetGameState());
	if (gameState)
	{
		if (gameState->OnKillPlayer.IsBound())
			gameState->OnKillPlayer.Broadcast();
	}
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

	if (!cameraLockTarget)
		SetCameraLockOff();
}

void APlayerCharacter::SetCameraLockOff()
{
	isLocking = false;
	cameraLockTarget = nullptr;
}