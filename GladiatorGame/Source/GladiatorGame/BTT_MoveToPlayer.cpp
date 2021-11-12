// Fill out your copyright notice in the Description page of Project Settings.

#include "BTT_MoveToPlayer.h"
#include "AIC_Enemy.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"

UBTT_MoveToPlayer::UBTT_MoveToPlayer(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("Move To Player");
}

EBTNodeResult::Type UBTT_MoveToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AEnemyCharacter* enemyCharacter = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	AAIController* enemyController = Cast<AAIController>(enemyCharacter->GetController());
	
	const APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("PlayerActor"));

	float playerSpeed = FVector::VectorPlaneProject(playerCharacter->GetVelocity(), FVector(0, 0, 1)).Size();

	enemyController->MoveToLocation(playerCharacter->GetActorLocation(), -1.f, true, true);

	return EBTNodeResult::Succeeded;

}

