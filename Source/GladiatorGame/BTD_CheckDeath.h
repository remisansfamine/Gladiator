// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTD_CheckDeath.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORGAME_API UBTD_CheckDeath : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public :
	UBTD_CheckDeath(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
};