// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Attack.h"
#include "AIController.h"
#include "AIC_Enemy.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTT_Attack::UBTT_Attack(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyCharacter* enemyCharacter = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	enemyCharacter->Attack();
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum("MovingState", 7);

	return EBTNodeResult::Succeeded;
}