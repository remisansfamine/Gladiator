// Fill out your copyright notice in the Description page of Project Settings.

#include "BTT_MoveToPlayer.h"
#include "AIC_Enemy.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "DrawDebugHelpers.h"

UBTT_MoveToPlayer::UBTT_MoveToPlayer(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("Move To Player");
}

FVector UBTT_MoveToPlayer::ProjectPointOnNavigableLocation(FVector desiredLocation, APawn* enemyPawn)
{
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(enemyPawn->GetWorld());
	if (!NavSys)
	{
		UE_LOG(LogTemp, Warning, TEXT("NavSys Failed"));
		return FVector::ZeroVector;
	}

	const FNavAgentProperties& AgentProps = enemyPawn->GetNavAgentPropertiesRef();
	FNavLocation target;

	NavSys->ProjectPointToNavigation(desiredLocation, target, INVALID_NAVEXTENT, &AgentProps);

	return target.Location;
}

EBTNodeResult::Type UBTT_MoveToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* cont = OwnerComp.GetAIOwner();

	APawn* enemyPawn = cont->GetPawn();
	if (!enemyPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("enemyPawn Failed"));
		return EBTNodeResult::Failed;
	}

	const AEnemyCharacter* enemyCharacter = Cast<AEnemyCharacter>(enemyPawn);
	if (targetAlreadySet == false)
	{
		currentTarget = enemyCharacter->GetActorLocation();
		targetAlreadySet = true;
	}

	AAIController* enemyController = Cast<AAIController>(enemyCharacter->GetController());
	
	const APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("PlayerActor"));
	if (!playerCharacter)
	{
		EBTNodeResult::Failed;
	}

	FVector enemyLocation = enemyPawn->GetActorLocation();
	FVector playerLocation = playerCharacter->GetActorLocation();
	FVector playerEnemyDir = enemyLocation - playerLocation;
	playerEnemyDir.Normalize();
	FVector pointInFrontPlayer = playerEnemyDir + playerLocation + playerEnemyDir * enemyCharacter->safePlayerDistanceMin;

	FVector projectedLocation = ProjectPointOnNavigableLocation(pointInFrontPlayer, enemyPawn);

	float distance = FVector::Dist(playerLocation, enemyLocation);
	float targetDistance = FVector::Dist(projectedLocation, currentTarget);

	DrawDebugSphere(enemyPawn->GetWorld(), projectedLocation, 10.f, 24, FColor::Red, false, 2.f);

	if (distance <= enemyCharacter->safePlayerDistanceMin)
	{
		enemyController->StopMovement();
		return EBTNodeResult::Failed;
	}
	else if (distance > enemyCharacter->safePlayerDistanceMin 
		&& targetDistance > enemyCharacter->safePlayerDistanceMax - enemyCharacter->safePlayerDistanceMin)
	{
		enemyController->MoveToLocation(projectedLocation);
		currentTarget = projectedLocation;
	}
	else
		return EBTNodeResult::Failed;

	return EBTNodeResult::Succeeded;

}

