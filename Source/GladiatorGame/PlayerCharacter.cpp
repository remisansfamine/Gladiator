// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "EnemyCharacter.h"
#include "LifeComponent.h"
#include "Camera\CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

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
	PlayerInputComponent->BindAction("LockOn", IE_Released, this, &APlayerCharacter::LockOn);

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
	FRotator ForwardRotation = (target->GetActorLocation() - GetActorLocation()).Rotation();
	SetActorRotation(ForwardRotation);

	FRotator BaseLock = GetController()->GetControlRotation();
	//GetController()->Rotation

	FVector Forward = target->GetActorLocation() - GetActorLocation();

	FQuat lookAtQuat = FRotationMatrix::MakeFromXZ(Forward, FVector::UpVector).ToQuat();
	GetFollowCamera()->SetWorldRotation(lookAtQuat, true);
}

void APlayerCharacter::LockOn()
{
	isLockingOn = !isLockingOn;

	if (!isLockingOn)
	{
		target = nullptr;
		return;
	}

	target = Cast<AEnemyCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyCharacter::StaticClass()));
}
