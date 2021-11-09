// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_MoveToBack.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORGAME_API UBTT_MoveToBack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public :
	UBTT_MoveToBack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
