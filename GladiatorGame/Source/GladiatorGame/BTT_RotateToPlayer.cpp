// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_RotateToPlayer.h"
#include "AIC_Enemy.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Controller.h"

UBTT_RotateToPlayer::UBTT_RotateToPlayer(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("Rotate To Player");
}

EBTNodeResult::Type UBTT_RotateToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	int enumId = OwnerComp.GetBlackboardComponent()->GetValueAsEnum("MovingState");

	const AAIController* cont = OwnerComp.GetAIOwner();
	AEnemyCharacter* enemyCharacter = Cast<AEnemyCharacter>(cont->GetPawn());
	AAIController* enemyController = Cast<AAIController>(enemyCharacter->GetController());
	const APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("PlayerActor"));


	if (enumId != 4 && enumId != 2)
	{
		enemyController->bSetControlRotationFromPawnOrientation = 1;
		return EBTNodeResult::Succeeded;
	}

	enemyController->bSetControlRotationFromPawnOrientation = 0;

	FQuat quaternion = FQuat::FindBetweenVectors(enemyCharacter->GetActorLocation(), playerCharacter->GetActorLocation());

	FRotator rotator = FRotator::MakeFromEuler(FVector(0, 0, quaternion.Z));
	enemyCharacter->SetActorRotation(quaternion, ETeleportType::None);

	return EBTNodeResult::Succeeded;
}

