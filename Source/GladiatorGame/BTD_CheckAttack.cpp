// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_CheckAttack.h"
#include "BehaviorTree/BlackboardComponent.h"

//#include "BehaviorTree/BehaviorTreeComponent.h"

UBTD_CheckAttack::UBTD_CheckAttack(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("CheckAttack");
}

bool UBTD_CheckAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (OwnerComp.GetBlackboardComponent()->GetValueAsEnum("MovingState") == 6)
		return true;

	return false;
}