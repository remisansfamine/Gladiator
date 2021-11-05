// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "LifeComponent.h"

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

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::Attack);

	PlayerInputComponent->BindAction<FDefend>("Defend", IE_Pressed, this, &APlayerCharacter::Defend, true);
	PlayerInputComponent->BindAction<FDefend>("Defend", IE_Released, this, &APlayerCharacter::Defend, false);
}

