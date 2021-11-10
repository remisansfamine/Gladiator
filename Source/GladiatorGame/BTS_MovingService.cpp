// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_MovingService.h"
#include "EnemyCharacter.h"
#include "PlayerCharacter.h"
#include "AIC_Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTS_MovingService::UBTS_MovingService(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("Moving Service");
}

void UBTS_MovingService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const AEnemyCharacter* enemyCharacter = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	const APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("PlayerActor"));

	float distance = FVector::Dist(enemyCharacter->GetActorLocation(), playerCharacter->GetActorLocation());
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat("Distance", distance);

	OwnerComp.GetBlackboardComponent()->SetValueAsFloat("DeltaTime", DeltaSeconds);
}