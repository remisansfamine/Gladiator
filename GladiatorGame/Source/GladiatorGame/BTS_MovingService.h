// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_MovingService.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORGAME_API UBTS_MovingService : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTS_MovingService(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
