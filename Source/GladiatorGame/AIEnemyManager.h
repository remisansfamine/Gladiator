// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIEnemyManager.generated.h"

class AAIC_Enemy;

UCLASS()
class GLADIATORGAME_API AAIEnemyManager : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Settings, meta = (AllowPrivateAccess = "true"))
	TArray<AAIC_Enemy*> enemies;
	
	int lastEnemyIndex = -1;

	void GetAllEnemyInRadius(TArray<int>& indexs);
	int RandomEnemy();
	int ClosestEnemy();
	int LastEnemy();

	void LaunchAttack();

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		float safePlayerDistanceMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		float safePlayerDistanceMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		float attackDelay;
	// Sets default values for this actor's properties
	AAIEnemyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
