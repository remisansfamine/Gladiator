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


FVector UBTT_MoveToPlayer::GetPointRadiusOnNavigableLocation(FVector originLocation, float radius, APawn* enemyPawn)
{
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(enemyPawn->GetWorld());
	if (!NavSys)
	{
		UE_LOG(LogTemp, Warning, TEXT("NavSys Failed"));
	}
	FNavLocation target;

	NavSys->GetRandomReachablePointInRadius(originLocation, radius, target);
	UE_LOG(LogTemp, Warning, TEXT("target location = (%f,%f,%f)"), target.Location.X, target.Location.Y, target.Location.Z);

	return target.Location;
}

FVector GetRandomPointInSemiTorus(float radiusMin, float radiusMax, FVector unitAxisB)
{
	FVector unitAxisA(0, 1, 0);

	float randomAngle = FMath::RandRange(0.f, 6.28f);

	float cX = FMath::Sin(randomAngle);
	float cY = FMath::Cos(randomAngle);

	FVector ringPos = FVector(cX, cY, 0);
	ringPos *= radiusMax;

	FVector2D sPos = FMath::RandPointInCircle(radiusMin);
	FVector result = (ringPos + FVector(sPos.X, sPos.Y, 0));
	result.Y = FMath::Abs(result.Y);

	FQuat quaternionResult = FQuat::FindBetweenVectors(unitAxisA, unitAxisB);

	return quaternionResult.RotateVector(result);
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
	AAIController* enemyController = Cast<AAIController>(enemyCharacter->GetController());
	
	const APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("PlayerActor"));
	if (!playerCharacter)
	{
		EBTNodeResult::Failed;
	}

	FVector enemyLocation = enemyPawn->GetActorLocation();
	FVector playerLocation = playerCharacter->GetActorLocation();

	float distance = FVector::Dist(playerLocation, enemyLocation);
	float playerSpeed = FVector::VectorPlaneProject(playerCharacter->GetVelocity(), FVector(0, 0, 1)).Size();

	if (distance > enemyCharacter->safePlayerDistanceMin && playerSpeed > 0)
	{
		enemyController->MoveToLocation(playerLocation);
	}
	else if (distance > enemyCharacter->safePlayerDistanceMax && enemyController->GetMoveStatus() == EPathFollowingStatus::Idle)
	{
		FVector playerEnemyDir = enemyLocation - playerLocation;
		playerEnemyDir.Normalize();

		FVector randomLocation = GetRandomPointInSemiTorus(enemyCharacter->safePlayerDistanceMin, 
			enemyCharacter->safePlayerDistanceMax, playerEnemyDir);
		UE_LOG(LogTemp, Warning, TEXT("randomLocation = (%f,%f,%f)"), randomLocation.X, randomLocation.Y, randomLocation.Z);

		FVector projectedLocation = ProjectPointOnNavigableLocation(randomLocation + playerLocation, enemyPawn);
		DrawDebugSphere(enemyPawn->GetWorld(), projectedLocation, 10.f, 24, FColor::Red, false, 1000000.f);
		
		enemyController->MoveToLocation(projectedLocation);
	}
	else
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;

}

