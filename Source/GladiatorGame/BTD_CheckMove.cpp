// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_CheckMove.h"
#include "LifeComponent.h"
#include "EnemyCharacter.h"
#include "PlayerCharacter.h"
#include "AIC_Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_CheckMove::UBTD_CheckMove(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("Check Move To Player");
}

bool UBTD_CheckMove::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const AAIController* cont = OwnerComp.GetAIOwner();

	APawn* enemyPawn = cont->GetPawn();
	const AEnemyCharacter* enemyCharacter = Cast<AEnemyCharacter>(enemyPawn);
	const APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("PlayerActor"));

	float distance = FVector::Dist(enemyCharacter->GetActorLocation(), playerCharacter->GetActorLocation());
	if (distance <= enemyCharacter->safePlayerDistanceMin)
	{
		AAIController* enemyController = Cast<AAIController>(enemyCharacter->GetController());
		enemyController->StopMovement();
		return false;
	}

	float playerSpeed = FVector::VectorPlaneProject(playerCharacter->GetVelocity(), FVector(0, 0, 1)).Size();
	if (playerSpeed == 0)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum("MovingState", 1);
		return false;
	}

	return true;
}
