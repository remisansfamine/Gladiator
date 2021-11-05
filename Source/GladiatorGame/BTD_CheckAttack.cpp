// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_CheckAttack.h"
#include "LifeComponent.h"
#include "EnemyCharacter.h"
#include "AIC_Enemy.h"

//#include "BehaviorTree/BehaviorTreeComponent.h"

UBTD_CheckAttack::UBTD_CheckAttack(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("CheckAttack");
}

bool UBTD_CheckAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{

	const AAIController* cont = OwnerComp.GetAIOwner();
	if (const AEnemyCharacter* character = Cast<AEnemyCharacter>(cont->GetPawn()))
		return character->lifeComponent->GetLife() != 5 ? true : false;

	//int life = cont->GetBlackboardComponent()->GetValueAsInt(GetSelectedBlackboardKey());

	return false;
}