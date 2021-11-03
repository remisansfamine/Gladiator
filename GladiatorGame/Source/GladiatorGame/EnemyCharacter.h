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

public :
	AEnemyCharacter();
	
	UCapsuleComponent* capsuleComponent;

private :
	UFUNCTION(BlueprintCallable)
	void OnHit();
};
