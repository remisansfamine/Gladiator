// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_CheckPlayerDistance.h"
#include "EnemyCharacter.h"
#include "PlayerCharacter.h"
#include "AIC_Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTS_CheckPlayerDistance::UBTS_CheckPlayerDistance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("Check Distance");
}

bool UBTS_CheckPlayerDistance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	int enumId = OwnerComp.GetBlackboardComponent()->GetValueAsEnum("MovingState");
	float distance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat("Distance");

	const AEnemyCharacter* enemyCharacter = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	AAIController* enemyController = Cast<AAIController>(enemyCharacter->GetController());

	if (enumId != 5)
	{
		if (distance <= enemyCharacter->safePlayerDistanceMin)
		{
			UE_LOG(LogTemp, Warning, TEXT("distance Failed, Distance = %f"), distance);

			enemyController->StopMovement();
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum("MovingState", 5);
			return false;
		}
	}
	else //GoBack
	{
		float distanceMin = enemyCharacter->safePlayerDistanceMin + 
			(enemyCharacter->safePlayerDistanceMax - enemyCharacter->safePlayerDistanceMin) / 2;

		if (distance > distanceMin)
		{
			enemyController->StopMovement();
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum("MovingState", 0);
			return true;
		}

		return false;
	}

	return true;
}

