// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_CheckAttackState.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_CheckAttackState::UBTD_CheckAttackState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("Check Attack State");
}

bool UBTD_CheckAttackState::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	int enumId = OwnerComp.GetBlackboardComponent()->GetValueAsEnum("MovingState");
	if (enumId == 6 || enumId == 7)
		return false;

	return true;
}
