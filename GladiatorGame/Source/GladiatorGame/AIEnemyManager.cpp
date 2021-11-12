// Fill out your copyright notice in the Description page of Project Settings.


#include "AIEnemyManager.h"
#include "AIC_Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
AAIEnemyManager::AAIEnemyManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAIEnemyManager::BeginPlay()
{
	Super::BeginPlay();
	LaunchAttackDelay();
}

void AAIEnemyManager::GetAllEnemyInRadius(TArray<int>& indexs)
{
	for (int i = 0; i < enemies.Num(); i++)
	{
		if (enemies[i]->GetBlackboardComponent()->GetValueAsFloat("Distance") < safePlayerDistanceMax)
			indexs.Add(i);
	}
}

int AAIEnemyManager::RandomEnemy()
{
	TArray<int> indexs;
	GetAllEnemyInRadius(indexs);

	if (indexs.Num() == 0)
		return -1;

	return FMath::FRandRange(0, indexs.Num());
}

int AAIEnemyManager::ClosestEnemy()
{
	int index = -1;
	float minDistance = 999999.f;

	for (int i = 0; i < enemies.Num(); i++)
	{
		float distance = enemies[i]->GetBlackboardComponent()->GetValueAsFloat("Distance");
		if (minDistance >= distance)
		{
			index = i;
			minDistance = distance;
		}
	}

	if (minDistance > safePlayerDistanceMax)
		return -1;

	return index;
}

int AAIEnemyManager::LastEnemy()
{
	
	if (lastEnemyIndex == -1 || enemies[lastEnemyIndex]->GetBlackboardComponent()->GetValueAsFloat("Distance") > safePlayerDistanceMax)
		return -1;

	return lastEnemyIndex;
}

void AAIEnemyManager::LaunchAttackDelay()
{
	FTimerHandle outHandleTime; 
	GetWorldTimerManager().SetTimer(outHandleTime, this, &AAIEnemyManager::LaunchAttack, attackDelay, false);
}

void AAIEnemyManager::LaunchAttack()
{
	if (enemies.Num() == 0)
	{
		LaunchAttackDelay();
		return;
	}
	int rand = FMath::RandRange(0, 2);
	int id;

	switch (rand)
	{
	case 0:
		id = ClosestEnemy();
		break;
	case 1:
		id = RandomEnemy();
		break;
	case 2:
		id = LastEnemy();
		break;
	}

	if (id == -1)
	{
		LaunchAttackDelay();
		return;
	}

	enemies[id]->LaunchAttack();
	lastEnemyIndex = id;
}

void AAIEnemyManager::AttackTerminated()
{
	LaunchAttackDelay();
}

void AAIEnemyManager::AddEnemy(AAIC_Enemy* enemyController)
{
	enemies.Add(enemyController);
}

void AAIEnemyManager::DeleteEnemy(AAIC_Enemy* enemyController)
{
	enemies.Remove(enemyController);
}

// Called every frame
void AAIEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

