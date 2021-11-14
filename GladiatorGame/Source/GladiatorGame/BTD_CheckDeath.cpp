// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_CheckDeath.h"
#include "PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_CheckDeath::UBTD_CheckDeath(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("Check Death");
}

bool UBTD_CheckDeath::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (OwnerComp.GetBlackboardComponent()->GetValueAsEnum("MovingState") == 8)
		return false;

	return true;
}