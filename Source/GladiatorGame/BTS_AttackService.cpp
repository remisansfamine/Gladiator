// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_AttackService.h"
#include "AIController.h"
#include "EnemyCharacter.h"
#include "PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTS_AttackService::UBTS_AttackService(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("AttackService");
}

void UBTS_AttackService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//AAIController* cont = OwnerComp.GetAIOwner();
	//
	//if (const AEnemyCharacter* character = Cast<AEnemyCharacter>(cont->GetPawn()))
	//	cont->GetBlackboardComponent()->SetValueAsString(GetSelectedBlackboardKey(), TEXT("TEST"));

	const AAIController* cont = OwnerComp.GetAIOwner();

	const AEnemyCharacter* enemyCharacter = Cast<AEnemyCharacter>(cont->GetPawn());
	const APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("PlayerActor"));

	if (!enemyCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Enemy!"));
		return;
	}

	if (!playerCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Player!"));
		return;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsFloat("Distance", FVector::Distance(playerCharacter->GetActorLocation(), enemyCharacter->GetActorLocation()));
}
