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
}

void AAIEnemyManager::GetAllEnemyInRadius(TArray<int>& indexs)
{
	for (int i = 0; i < enemies.GetTypeSize(); i++)
	{
		if (enemies[i]->GetBlackboardComponent()->GetValueAsFloat("Distance") < safePlayerDistanceMax)
			indexs.Add(i);
	}
}

int AAIEnemyManager::RandomEnemy()
{
	TArray<int> indexs;
	GetAllEnemyInRadius(indexs);

	if (indexs.GetTypeSize() == 0)
		return -1;

	return FMath::FRandRange(0, indexs.GetTypeSize());
}

int AAIEnemyManager::ClosestEnemy()
{
	int index = -1;
	float minDistance = -1.f;

	for (int i = 0; i < enemies.GetTypeSize(); i++)
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

}

// Called every frame
void AAIEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

