// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_MoveToBack.h"
#include "AIC_Enemy.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"
#include "BTT_PlaceAroundPlayer.h"

UBTT_MoveToBack::UBTT_MoveToBack(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("Move To Back");
}

EBTNodeResult::Type UBTT_MoveToBack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
		return EBTNodeResult::Failed;
	}

	FVector enemyLocation = enemyPawn->GetActorLocation();
	FVector playerLocation = playerCharacter->GetActorLocation();
	FVector playerEnemyDir = enemyLocation - playerLocation;
	playerEnemyDir.Normalize();

	FVector endLocation = enemyLocation + playerEnemyDir * 200;

	FHitResult hit;
	FVector projectedLocation;
	if (enemyPawn->GetWorld()->LineTraceSingleByObjectType(hit, enemyLocation, endLocation, FCollisionObjectQueryParams::AllStaticObjects))
	{
		projectedLocation = ProjectPointOnNavigableLocation(hit.Location, enemyPawn);
	}
	else
	{
		projectedLocation = ProjectPointOnNavigableLocation(endLocation, enemyPawn);
	}

	enemyController->MoveToLocation(projectedLocation, 100);

	return EBTNodeResult::Succeeded;
}

