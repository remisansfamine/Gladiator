// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Attack.h"
#include "AIController.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTT_Attack::UBTT_Attack(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//get NPC
	const AAIController* cont = OwnerComp.GetAIOwner();

	FString debug = cont->GetBlackboardComponent()->GetValueAsString(GetSelectedBlackboardKey());

	//test to see if the NPC class supports the ICombatInterface interface
	if (const AEnemyCharacter* npc = Cast<AEnemyCharacter>(cont->GetPawn()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, debug);
	}

	return EBTNodeResult::Succeeded;
}