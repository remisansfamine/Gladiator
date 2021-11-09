// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "LifeComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

APlayerCharacter::APlayerCharacter() 
	: AGladiatorGameCharacter()
{
	lifeComponent->SetLife(5);
	lifeComponent->invicibleCooldown = 1.f;
	ActivateCamera();
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("LockOn", IE_Pressed, this, &APlayerCharacter::LockOn);
	PlayerInputComponent->BindAction("LockOn", IE_Released, this, &APlayerCharacter::LockOff);

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

	if (target)
		LookAtTarget();
}

void APlayerCharacter::LookAtTarget()
{
	FVector differenceVector = target->GetActorLocation() - GetActorLocation();

	FRotator lookAtRot = differenceVector.Rotation();
	lookAtRot = UKismetMathLibrary::RInterpTo(GetActorRotation(), lookAtRot, GetWorld()->GetDeltaSeconds(), lockOnSpeed);

	SetActorRotation(lookAtRot);

	FRotator controllerLookAt = Controller->GetControlRotation();
	controllerLookAt.Yaw = lookAtRot.Yaw;
	Controller->SetControlRotation(controllerLookAt);
}

void APlayerCharacter::LockOn()
{
	target = GetOtherGladiator(minLockOnDistance, maxLockOnDistance);

	if (target)
		CameraBoom->bUsePawnControlRotation = false; // Rotate the arm based on the controller
}

void APlayerCharacter::LockOff()
{
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	target = nullptr;
}