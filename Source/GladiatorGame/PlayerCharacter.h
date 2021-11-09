// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GladiatorGameCharacter.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORGAME_API APlayerCharacter : public AGladiatorGameCharacter
{
	GENERATED_BODY()

	AGladiatorGameCharacter* target;

	void LookAtTarget();

	UPROPERTY(EditAnywhere)
	float lockOnSpeed = 2.5f;

	UPROPERTY(EditAnywhere)
	float minLockOnDistance = 0.f;

	UPROPERTY(EditAnywhere)
	float maxLockOnDistance = 500.f;

public :
	APlayerCharacter();

	UFUNCTION()
	void LockOn();

	UFUNCTION()
	void LockOff();

	void Tick(float DeltaTime) override;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// End of APawn interface
	DECLARE_DELEGATE_OneParam(FDefend, bool);
};
