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

	bool isLockingOn;

	AGladiatorGameCharacter* target;

	void LookAtTarget();

public :
	APlayerCharacter();

	UFUNCTION()
	void LockOn();

	void Tick(float DeltaTime) override;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// End of APawn interface
	DECLARE_DELEGATE_OneParam(FDefend, bool);
};
