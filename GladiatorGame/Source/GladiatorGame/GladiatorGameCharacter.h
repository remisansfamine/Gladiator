// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterState.h"
#include "Blueprint/UserWidget.h"
#include "GladiatorGameCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterState, ECharacterState, characterState);

UCLASS(config=Game)
class AGladiatorGameCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* hammer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* attackCollider;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* shield;

	void TakeDamage(int damage, const FVector& senderPosition);

public:
	AGladiatorGameCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:
	UFUNCTION()
	void OnInvicibilityStop();

	UFUNCTION()
	void OnHurt();

	UFUNCTION()
	void OnDeath();

	void ActivateCamera();
	void DeactivateCamera();

	UFUNCTION(BlueprintCallable)
	void SetAttackState(bool attacking);

	UFUNCTION()
	void OverlapCallback(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void Move(EAxis::Type axis, float value);
	void Move(const FVector& direction, float value);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, meta = (AllowPrivateAccess = "true"))
	ECharacterState characterState = ECharacterState::IDLE;

	void SetState(ECharacterState state);

	UFUNCTION(BlueprintCallable)
	virtual void DefendOn();

	UFUNCTION(BlueprintCallable)
	virtual void DefendOff();

	UFUNCTION(BlueprintCallable)
	virtual void Idle();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	virtual void BeginPlay() override;

	void setCameraShake(const TSubclassOf<UCameraShakeBase>& shakeClass, float scale);

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMatineeCameraShake> camShake;

	AGladiatorGameCharacter* GetOtherGladiator(float minDistance, float maxDistance);

	void LookAtTarget(AActor* target, float lookSpeed);

public:
	bool canDefend() { return characterState == ECharacterState::IDLE || characterState == ECharacterState::ATTACKING; }
	bool canAttack() { return characterState == ECharacterState::IDLE; }
	bool canMove() { return characterState == ECharacterState::DEFENDING || characterState == ECharacterState::IDLE; }
	bool isAlive() { return characterState != ECharacterState::DEAD;  }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Life", meta = (AllowPrivateAccess = "true"))
	class ULifeComponent* healthComponent;

	UFUNCTION(BlueprintCallable)
	virtual void Attack();

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }	

	UPROPERTY(BlueprintAssignable, Category = "Character|State")
	FCharacterState OnStateChanged;
};
