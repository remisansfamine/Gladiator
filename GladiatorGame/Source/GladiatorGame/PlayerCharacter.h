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

	AGladiatorGameCharacter* cameraLockTarget;

	UPROPERTY(EditAnywhere)
	float lockOnSpeed = 2.5f;

	UPROPERTY(EditAnywhere)
	float minLockOnDistance = 0.f;

	UPROPERTY(EditAnywhere)
	float maxLockOnDistance = 500.f;

	bool isLocking = false;

public:
	APlayerCharacter();

	void CameraLock();

	UFUNCTION()
	void SetCameraLock();

	void SetCameraLockOn();
	void SetCameraLockOff();

	void Tick(float DeltaTime) override;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// End of APawn interface
	DECLARE_DELEGATE_OneParam(FDefend, bool);
};
