// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_MoveToPlayer.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORGAME_API UBTT_MoveToPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public :
	UBTT_MoveToPlayer(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	FVector ProjectPointOnNavigableLocation(FVector desiredLocation, APawn* enemyPawn);
	FVector GetPointRadiusOnNavigableLocation(FVector originLocation, float radius, APawn* enemyPawn);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

FVector GetRandomPointInSemiTorus(float radiusMin, float radiusMax, FVector unitAxisB);
