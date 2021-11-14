// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GladiatorGameCharacter.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORGAME_API AEnemyCharacter : public AGladiatorGameCharacter
{
	GENERATED_BODY()

	UFUNCTION()
	void OnDeathEnemy();

public :
	AEnemyCharacter();
	
	UCapsuleComponent* capsuleComponent;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	float rotateSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	float wantedRoomRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	float attackDistance;

private :
	class APlayerCharacter* playerCharacter;
};
