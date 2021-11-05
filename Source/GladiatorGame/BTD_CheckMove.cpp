// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_CheckMove.h"
#include "LifeComponent.h"
#include "EnemyCharacter.h"
#include "AIC_Enemy.h"

UBTD_CheckMove::UBTD_CheckMove(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("Check Move");
}

bool UBTD_CheckMove::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{

	const AAIController* cont = OwnerComp.GetAIOwner();
	if (const AEnemyCharacter* character = Cast<AEnemyCharacter>(cont->GetPawn()))
		return character->lifeComponent->GetLife() != 5 ? true : false;

	//int life = cont->GetBlackboardComponent()->GetValueAsInt(GetSelectedBlackboardKey());

	return false;
}
