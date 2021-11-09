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
	const AAIController* cont = OwnerComp.GetAIOwner();
	APawn* enemyPawn = cont->GetPawn();
	const AEnemyCharacter* enemyCharacter = Cast<AEnemyCharacter>(enemyPawn);
	const APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("PlayerActor"));

	int enumId = OwnerComp.GetBlackboardComponent()->GetValueAsEnum("MovingState");
	float distance = FVector::Dist(enemyCharacter->GetActorLocation(), playerCharacter->GetActorLocation());
	AAIController* enemyController = Cast<AAIController>(enemyCharacter->GetController());

	if (enumId != 4)
	{
		if (distance <= enemyCharacter->safePlayerDistanceMin)
		{
			UE_LOG(LogTemp, Warning, TEXT("distance Failed, Distance = %f"), distance);

			enemyController->StopMovement();
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum("MovingState", 4);
			return false;
		}
	}
	else
	{
		if (distance > enemyCharacter->safePlayerDistanceMin + (enemyCharacter->safePlayerDistanceMax - enemyCharacter->safePlayerDistanceMin)/2)
		{
			enemyController->StopMovement();
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum("MovingState", 0);
			return true;
		}

		return false;
	}

	return true;
}

