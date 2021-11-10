// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_CheckPlacing.h"
#include "EnemyCharacter.h"
#include "PlayerCharacter.h"
#include "AIC_Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "BTT_PlaceAroundPlayer.h"

UBTD_CheckPlacing::UBTD_CheckPlacing(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("Check Placing");
}

bool UBTD_CheckPlacing::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	int enumId = OwnerComp.GetBlackboardComponent()->GetValueAsEnum("MovingState");
	if (enumId == 1)
		return false;

	const AAIController* cont = OwnerComp.GetAIOwner();
	APawn* enemyPawn = cont->GetPawn();
	const AEnemyCharacter* enemyCharacter = Cast<AEnemyCharacter>(enemyPawn);
	const APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("PlayerActor"));

	if (enumId != 5 && enumId != 1)
	{
		float playerSpeed = FVector::VectorPlaneProject(playerCharacter->GetVelocity(), FVector(0, 0, 1)).Size();
		if (playerSpeed > 0 && FVector::Dist(enemyCharacter->GetActorLocation(), playerCharacter->GetActorLocation()) > enemyCharacter->safePlayerDistanceMax)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum("MovingState", 1);
			return false;
		}
	}


	if (enumId == 0) //Idle
	{
		FVector desiredTarget = ProjectPointOnNavigableLocation(enemyCharacter->GetActorLocation(), enemyPawn);
		float distance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat("Distance");
		
		if (distance < enemyCharacter->safePlayerDistanceMax && distance > enemyCharacter->safePlayerDistanceMin)
		{
			if (checkIfPawnIsInSphere(enemyCharacter->wantedRoomRadius, desiredTarget, enemyPawn))
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum("MovingState", 2);
				return true;
			}
		
			if (checkIfPawnEnemyIsFront(enemyCharacter->GetActorLocation(), playerCharacter->GetActorLocation(), enemyPawn))
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum("MovingState", 2);
				return true;
			}
		
		
			OwnerComp.GetBlackboardComponent()->SetValueAsVector("currentTarget", desiredTarget);
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum("MovingState", 4);
			return false;
		}

		OwnerComp.GetBlackboardComponent()->SetValueAsEnum("MovingState", 2);
		return true;
	}
	else if (enumId == 3) //Placing
	{
		FVector currentTarget = OwnerComp.GetBlackboardComponent()->GetValueAsVector("currentTarget");

		if (checkIfPawnIsInSphere(enemyCharacter->wantedRoomRadius, currentTarget, enemyPawn))
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum("MovingState", 2);
			return true;
		}

		AAIController* enemyController = Cast<AAIController>(enemyCharacter->GetController());
		if (enemyController->GetMoveStatus() == EPathFollowingStatus::Idle)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum("MovingState", 4);
			return false;
		}

		return false;
	}
	else if (enumId == 4) //Placed
	{
		FVector currentTarget = OwnerComp.GetBlackboardComponent()->GetValueAsVector("currentTarget");
		
		if (checkIfPawnEnemyIsFront(enemyCharacter->GetActorLocation(), playerCharacter->GetActorLocation(), enemyPawn))
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum("MovingState", 2);
			return true;
		}
				
		return false;
	}

	return true;
}

bool checkIfPawnIsInSphere(float radius, const FVector& center, APawn* ownPawn)
{
	TArray<FOverlapResult> overlaps;

	if (ownPawn->GetWorld()->OverlapMultiByObjectType(overlaps, center, FQuat::Identity, 
		FCollisionObjectQueryParams::AllObjects, FCollisionShape::MakeSphere(radius)))
	{
		DrawDebugSphere(ownPawn->GetWorld(), center, radius, 6, FColor::Green, false, 5.f);

		for (FOverlapResult overlap : overlaps)
		{
			if (APawn* pawnCast = Cast<APawn>(overlap.Actor))
			{
				if (pawnCast != ownPawn)
				{
					if (AEnemyCharacter* enemyCast = Cast<AEnemyCharacter>(pawnCast))
					{
						AAIController* enemyController = Cast<AAIController>(enemyCast->GetController());
						if (enemyController->GetBlackboardComponent()->GetValueAsEnum("MovingState") == 4)
							return true;
					}
				}
			}
		}
	}
	return false;
}

bool checkIfPawnEnemyIsFront(const FVector& start, const FVector& end, const APawn* ownPawn)
{
	TArray<FHitResult> hits;
	if (ownPawn->GetWorld()->LineTraceMultiByObjectType(hits, start, end, FCollisionObjectQueryParams::AllObjects))
	{
		for (FHitResult hit : hits)
		{
			if (APawn* pawnCast = Cast<APawn>(hit.Actor))
			{
				if (ownPawn != pawnCast)
				{
					if (AEnemyCharacter* enemyCast = Cast<AEnemyCharacter>(hit.Actor))
					{
						AAIController* enemyController = Cast<AAIController>(enemyCast->GetController());
						if (enemyController->GetBlackboardComponent()->GetValueAsEnum("MovingState") == 4)
							return true;
					}
				}
			}
		}
	}

	DrawDebugLine(ownPawn->GetWorld(), start, end, FColor::Green, false, 0.1f);

	return false;
}

