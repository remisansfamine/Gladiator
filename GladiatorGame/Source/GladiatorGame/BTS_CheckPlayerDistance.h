// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTS_CheckPlayerDistance.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORGAME_API UBTS_CheckPlayerDistance : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
public :
	UBTS_CheckPlayerDistance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
};
