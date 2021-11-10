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
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AGladiatorGameCharacter::AGladiatorGameCharacter()
{

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PawnIgnoreCam"));

	GetMesh()->SetCollisionProfileName(TEXT("CharacterMeshIgnoreCam"));
	GetMesh()->SetVectorParameterValueOnMaterials("FlickerColor", FVector(0.f, 0.f, 0.f));

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

	hammer = CreateDefaultSubobject<USkeletalMeshComponent>("Hammer");
	hammer->SetupAttachment(GetMesh(), TEXT("WeaponPoint"));

	weaponCollider = CreateDefaultSubobject<USphereComponent>("Sphere collider");
	weaponCollider->SetupAttachment(hammer, TEXT("ColliderSocket"));
	weaponCollider->SetGenerateOverlapEvents(true);

	shield = CreateDefaultSubobject<USkeletalMeshComponent>("Shield");
	shield->SetupAttachment(GetMesh(), TEXT("DualWeaponPoint"));

	lifeComponent = CreateDefaultSubobject<ULifeComponent>("LifeComponent");
	lifeComponent->OnHurt.AddDynamic(this, &AGladiatorGameCharacter::OnHurt);
	lifeComponent->OnKill.AddDynamic(this, &AGladiatorGameCharacter::OnDeath);
	lifeComponent->OnInvicibilityStop.AddDynamic(this, &AGladiatorGameCharacter::OnInvicibilityStop);

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
	if (!OverlappedComp || !OtherActor || OtherActor == this)
		return;

	AGladiatorGameCharacter* other = Cast<AGladiatorGameCharacter>(OtherActor);

	if (!other)
		return;

	ULifeComponent* otherLifeComp = other->lifeComponent;

	if (!other->lifeComponent)
		return;

	otherLifeComp->Hurt(1);
}

void AGladiatorGameCharacter::SetAttackState(bool attacking)
{
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

void AGladiatorGameCharacter::OnInvicibilityStop()
{
	GetMesh()->SetVectorParameterValueOnMaterials("FlickerColor", FVector(0.f, 0.f, 0.f));
}

void AGladiatorGameCharacter::setCameraShake(const TSubclassOf<UCameraShakeBase>& shakeClass, float scale)
{
	if (shakeClass)
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(shakeClass, scale);
}

void AGladiatorGameCharacter::OnHurt()
{
	GetMesh()->SetVectorParameterValueOnMaterials("FlickerColor", FVector(1.f, 0.f, 0.f));
	setCameraShake(camShake, 0.5f);
}

void AGladiatorGameCharacter::OnDeath()
{
	setCameraShake(camShake, 1.25f);

	SetAttackState(false);
	SetState(ECharacterState::IDLE);

	canMove = false;

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(TEXT("RagdollIgnoreCam"));
	GetMesh()->SetSimulatePhysics(true);

	hammer->DetachFromParent(true);
	hammer->SetSimulatePhysics(true);
	hammer->SetCollisionProfileName(TEXT("RagdollIgnoreCam"));

	shield->DetachFromParent(true);
	shield->SetSimulatePhysics(true);
	shield->SetCollisionProfileName(TEXT("RagdollIgnoreCam"));
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

AGladiatorGameCharacter* AGladiatorGameCharacter::GetOtherGladiator(float minDistance, float maxDistance)
{
	float minDistSquared = minDistance * minDistance;
	float maxDistSquared = maxDistance * maxDistance;

	FVector currentLocation = GetActorLocation();

	TArray<AActor*> gladiators;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGladiatorGameCharacter::StaticClass(), gladiators);

	TArray<AGladiatorGameCharacter*> validGladiators;
	for (AActor* actor : gladiators)
	{
		if (actor == this)
			continue;
		
		float distSquared = GetSquaredDistanceTo(actor);

		if (GetSquaredDistanceTo(actor) < minDistSquared || GetSquaredDistanceTo(actor) > maxDistSquared)
			continue;

		AGladiatorGameCharacter* gladiator = Cast<AGladiatorGameCharacter>(actor);

		validGladiators.Add(gladiator);
	}

	if (validGladiators.Num() == 0)
		return nullptr;

	validGladiators.Sort([this](const AGladiatorGameCharacter& A, const AGladiatorGameCharacter& B)
	{
		float distA = GetDistanceTo(&A);
		float distB = GetDistanceTo(&B);
		return distA < distB;
	});

	return validGladiators[0];
}

void AGladiatorGameCharacter::LookAtTarget(AActor* target, float lookSpeed)
{
	FVector toLookAt = target->GetActorLocation() - GetActorLocation();
	FRotator lookAtRot = toLookAt.Rotation();

	FRotator actorLookAt = GetActorRotation();
	FRotator controllerLookAt = Controller->GetControlRotation();
	controllerLookAt.Yaw = actorLookAt.Yaw = lookAtRot.Yaw;

	actorLookAt = UKismetMathLibrary::RInterpTo(GetActorRotation(), actorLookAt, GetWorld()->GetDeltaSeconds(), lookSpeed);
	controllerLookAt = UKismetMathLibrary::RInterpTo(Controller->GetControlRotation(), controllerLookAt, GetWorld()->GetDeltaSeconds(), lookSpeed);

	SetActorRotation(actorLookAt);
	Controller->SetControlRotation(controllerLookAt);
}