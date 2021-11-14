// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_CheckAttackDistance.h"
#include "EnemyCharacter.h"
#include "AIC_Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTD_CheckAttackDistance::UBTD_CheckAttackDistance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("Check Attack Distance");
}

bool UBTD_CheckAttackDistance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AEnemyCharacter* enemyCharacter = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	float distance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat("Distance");
	if (enemyCharacter->attackDistance >= distance)
	{
		AAIC_Enemy* enemyController = Cast<AAIC_Enemy>(enemyCharacter->GetController());
		enemyController->StopMovement();

		return false;
	}

	return true;
}
