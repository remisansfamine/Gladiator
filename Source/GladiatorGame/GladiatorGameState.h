// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GladiatorGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKill);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameTerminate, bool, victory);
/**
 * 
**/
UCLASS(config=Game, notplaceable, BlueprintType, Blueprintable)
class GLADIATORGAME_API AGladiatorGameState : public AGameStateBase
{
	GENERATED_BODY()
	
	UFUNCTION()
	void Defeat();
	void Victory();

	UFUNCTION()
	void OnEnemyDeath();

public :
	AGladiatorGameState();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	int enemiesCount;

	FKill OnKillPlayer;
	FKill OnKillEnemy;

	UPROPERTY(BlueprintAssignable, Category = "Components|GameSettings")
	FGameTerminate OnGameTerminate;


};
