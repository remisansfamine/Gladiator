// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIC_Enemy.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORGAME_API AAIC_Enemy : public AAIController
{
	GENERATED_BODY()
	
public :
	AAIC_Enemy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay();
	virtual void Tick(float deltaTime);

	virtual void OnPossess(APawn* const pawn);

	class UBlackboardComponent* GetBB() const;

	class AAIEnemyManager* aiEnemyManager;

	void FindAIEnemyManager();
	void LaunchAttack();

	UFUNCTION(BlueprintCallable)
	void AttackTerminated();

private :

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* behaviorTreeComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* btree;

	class UBlackboardComponent* blackboard;

};
