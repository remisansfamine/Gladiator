// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_AttackService.h"
#include "AIController.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTS_AttackService::UBTS_AttackService(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("AttackService");
}

void UBTS_AttackService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* cont = OwnerComp.GetAIOwner();

	if (const AEnemyCharacter* character = Cast<AEnemyCharacter>(cont->GetPawn()))
		cont->GetBlackboardComponent()->SetValueAsString(GetSelectedBlackboardKey(), TEXT("TEST"));
}
