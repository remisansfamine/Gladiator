// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_CheckPlacing.h"
#include "EnemyCharacter.h"
#include "PlayerCharacter.h"
#include "AIC_Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"

UBTD_CheckPlacing::UBTD_CheckPlacing(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("Check Placing");
}

bool UBTD_CheckPlacing::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	int enumId = OwnerComp.GetBlackboardComponent()->GetValueAsEnum("MovingState");
	if (enumId == 0)
		return false;

	const AAIController* cont = OwnerComp.GetAIOwner();
	APawn* enemyPawn = cont->GetPawn();
	const AEnemyCharacter* enemyCharacter = Cast<AEnemyCharacter>(enemyPawn);
	const APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("PlayerActor"));

	if (enumId == 1 || enumId == 2 || enumId == 3) //PlaceAroundPlayer
	{
		float playerSpeed = FVector::VectorPlaneProject(playerCharacter->GetVelocity(), FVector(0, 0, 1)).Size();
		if (playerSpeed > 0 && FVector::Dist(enemyCharacter->GetActorLocation(), playerCharacter->GetActorLocation()) > enemyCharacter->safePlayerDistanceMax)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum("MovingState", 0);
			return false;
		}
	}
	
	if (enumId == 2) //Placing
	{
		FVector currentTarget = OwnerComp.GetBlackboardComponent()->GetValueAsVector("currentTarget");

		if (checkIfPawnIsInSphere(80, enemyCharacter->GetActorLocation(), currentTarget, enemyPawn))
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum("MovingState", 1);
			return true;
		}

		AAIController* enemyController = Cast<AAIController>(enemyCharacter->GetController());
		if (enemyController->GetMoveStatus() == EPathFollowingStatus::Idle)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum("MovingState", 3);
			return false;
		}

		return false;
	}
	else if (enumId == 3) //Placed
	{
		FVector currentTarget = OwnerComp.GetBlackboardComponent()->GetValueAsVector("currentTarget");
		
		FHitResult hit;
		if (enemyPawn->GetWorld()->LineTraceSingleByChannel(hit, currentTarget, playerCharacter->GetActorLocation(), ECollisionChannel::ECC_Pawn))
		{
			if (AEnemyCharacter* pawnCast = Cast<AEnemyCharacter>(hit.Actor))
			{
				if (pawnCast != enemyPawn)
				{ 
					AAIController* enemyController = Cast<AAIController>(pawnCast->GetController());
					if (enemyController->GetBlackboardComponent()->GetValueAsEnum("MovingState") == 3)
						return true;
				}
				
			}
		}
		
		return false;
	}

	return true;
}

bool checkIfPawnIsInSphere(float radius, const FVector& start, const FVector& end, APawn* ownPawn)
{
	TArray<FHitResult> hits;
	if (ownPawn->GetWorld()->SweepMultiByChannel(hits, start,
		end, FQuat::Identity, ECollisionChannel::ECC_Pawn,
		FCollisionShape::MakeSphere(radius)))
	{
		DrawDebugSphere(ownPawn->GetWorld(), end, radius, 24, FColor::Green, false, 5.f);

		for (FHitResult hit : hits)
		{
			if (APawn* pawnCast = Cast<APawn>(hit.Actor))
			{
				if (pawnCast != ownPawn)
				{
					return true;
				}
			}
		}
	}
	return false;
}

