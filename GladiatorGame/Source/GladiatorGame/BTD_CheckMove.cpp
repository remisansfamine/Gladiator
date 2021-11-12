// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_CheckMove.h"
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
	if (OwnerComp.GetBlackboardComponent()->GetValueAsEnum("MovingState") != 1)
		return false;

	const APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("PlayerActor"));
	float playerSpeed = FVector::VectorPlaneProject(playerCharacter->GetVelocity(), FVector(0, 0, 1)).Size();
	
	if (playerSpeed == 0)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum("MovingState", 0);

		const AEnemyCharacter* enemyCharacter = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
		enemyCharacter->GetController()->StopMovement();

		return false;
	}

	return true;
}
