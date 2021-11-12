// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_AttackTerminated.h"
#include "AIController.h"
#include "AIC_Enemy.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_AttackTerminated::UBTT_AttackTerminated(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("Attack Terminated");
}

EBTNodeResult::Type UBTT_AttackTerminated::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* cont = OwnerComp.GetAIOwner();

	APawn* enemyPawn = cont->GetPawn();

	const AEnemyCharacter* enemyCharacter = Cast<AEnemyCharacter>(enemyPawn);
	AAIC_Enemy* enemyController = Cast<AAIC_Enemy>(enemyCharacter->GetController());

	enemyController->AttackTerminated();

	return EBTNodeResult::Succeeded;
}
