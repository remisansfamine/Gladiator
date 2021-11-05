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

	const AEnemyCharacter* enemyCharacter = Cast<AEnemyCharacter>(cont->GetPawn());
	if (!enemyCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("!Enemy"));
		return false;
	}

	const APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("PlayerActor"));
	if (!playerCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("!Player"));
		return false;
	}

	//USE Replace MoveTo by task -> MoveToLocation !!!!

	UE_LOG(LogTemp, Warning, TEXT("Distance between player and enemy = %f"),
		FVector::Distance(playerCharacter->GetActorLocation(), enemyCharacter->GetActorLocation()));

	return FVector::Distance(playerCharacter->GetActorLocation(), enemyCharacter->GetActorLocation()) > 500;
}
