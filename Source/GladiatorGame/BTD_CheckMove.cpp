// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_CheckMove.h"
#include "LifeComponent.h"
#include "EnemyCharacter.h"
#include "PlayerCharacter.h"
#include "AIC_Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_CheckMove::UBTD_CheckMove(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("Check Move");
}

bool UBTD_CheckMove::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const AAIController* cont = OwnerComp.GetAIOwner();

	APawn* enemyPawn = cont->GetPawn();
	if (!enemyPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("enemyPawn Failed"));
		return EBTNodeResult::Failed;
	}

	const AEnemyCharacter* enemyCharacter = Cast<AEnemyCharacter>(enemyPawn);
	AAIController* enemyController = Cast<AAIController>(enemyCharacter->GetController());

	const APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("PlayerActor"));
	if (!playerCharacter)
	{
		EBTNodeResult::Failed;
	}

	float distance = FVector::Dist(enemyCharacter->GetActorLocation(), playerCharacter->GetActorLocation());



	return true;
}
