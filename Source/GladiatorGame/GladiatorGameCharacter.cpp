// Copyright Epic Games, Inc. All Rights Reserved.

#include "GladiatorGameCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "LifeComponent.h"
#include "UObject/ConstructorHelpers.h"


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

	hammer = CreateDefaultSubobject<UStaticMeshComponent>("Hammer");
	hammer->SetupAttachment(GetMesh(), TEXT("WeaponPoint"));
	hammer->SetGenerateOverlapEvents(true);

	weaponCollider = CreateDefaultSubobject<USphereComponent>("Sphere collider");
	weaponCollider->SetupAttachment(hammer, TEXT("Socket"));
	weaponCollider->SetGenerateOverlapEvents(true);

	shield = CreateDefaultSubobject<UStaticMeshComponent>("Shield");
	shield->SetupAttachment(GetMesh(), TEXT("DualWeaponPoint"));

	lifeComponent = CreateDefaultSubobject<ULifeComponent>("LifeComponent");
	lifeComponent->OnKill.AddDynamic(this, &AGladiatorGameCharacter::OnDeath);

	canMove = true;
}

void AGladiatorGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (weaponCollider)
	{
		weaponCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		weaponCollider->OnComponentBeginOverlap.AddDynamic(this, &AGladiatorGameCharacter::OverlapCallback);
	}
}

void AGladiatorGameCharacter::OverlapCallback(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Overlap"));

	if (!OverlappedComp || !OtherActor || OtherActor == this)
		return;

	AGladiatorGameCharacter* other = Cast<AGladiatorGameCharacter>(OtherActor);

	if (!other)
		return;

	ULifeComponent* otherLifeComp = other->lifeComponent;

	if (!other->lifeComponent)
		return;

	otherLifeComp->Hurt(1);

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("hurt"));
}

void AGladiatorGameCharacter::SetAttackState(bool attacking)
{
	if (attacking)
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Attack set true"));

	weaponCollider->SetCollisionEnabled(attacking ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
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
	canMove = false;

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
}

void AGladiatorGameCharacter::SetState(ECharacterState state)
{
	characterState = state;

	if (OnStateChanged.IsBound())
		OnStateChanged.Broadcast(state);
}

void AGladiatorGameCharacter::Attack()
{
	if (characterState == ECharacterState::ATTACKING)
		return;

	canMove = false;
	SetState(ECharacterState::ATTACKING);
}

void AGladiatorGameCharacter::Defend(bool defending)
{
	if (characterState == ECharacterState::ATTACKING)
		return;

	canMove = true;
	SetState(defending ? ECharacterState::DEFENDING : ECharacterState::IDLE);
}

void AGladiatorGameCharacter::Idle()
{
	canMove = true;
	SetState(ECharacterState::IDLE);
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
	if (!canMove || value == 0.0f)
		return;

	AddMovementInput(direction, value);
}