// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_RotateToPlayer.h"
#include "AIC_Enemy.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetMathLibrary.h"

#include "GameFramework/CharacterMovementComponent.h"


UBTT_RotateToPlayer::UBTT_RotateToPlayer(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("Rotate To Player");
}

EBTNodeResult::Type UBTT_RotateToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	int enumId = OwnerComp.GetBlackboardComponent()->GetValueAsEnum("MovingState");

	AEnemyCharacter* enemyCharacter = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	AAIController* enemyController = Cast<AAIController>(enemyCharacter->GetController());
	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("PlayerActor"));

	if (enumId == 5 || enumId == 4)
	{
		float deltaTime = OwnerComp.GetBlackboardComponent()->GetValueAsFloat("DeltaTime");
		FRotator lookAt = UKismetMathLibrary::FindLookAtRotation(enemyCharacter->GetActorLocation(), playerCharacter->GetActorLocation());
		FRotator rotator = UKismetMathLibrary::RInterpTo(enemyCharacter->GetActorRotation(), lookAt, deltaTime, enemyCharacter->rotateSpeed);

		enemyCharacter->SetActorRotation(rotator.Quaternion());
	}

	return EBTNodeResult::Succeeded;
}

