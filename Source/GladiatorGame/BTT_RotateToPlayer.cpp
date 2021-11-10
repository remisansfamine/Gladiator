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

	AAIController* cont = OwnerComp.GetAIOwner();
	AEnemyCharacter* enemyCharacter = Cast<AEnemyCharacter>(cont->GetPawn());
	AAIController* enemyController = Cast<AAIController>(enemyCharacter->GetController());
	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("PlayerActor"));


	if (enumId != 4 && enumId != 3)
	{
		enemyController->ClearFocus(EAIFocusPriority::Move);
		enemyCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
		return EBTNodeResult::Succeeded;
	}

	enemyCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	FRotator lookAt = UKismetMathLibrary::FindLookAtRotation(enemyCharacter->GetActorLocation(), playerCharacter->GetActorLocation());
	FRotator rotator = UKismetMathLibrary::RInterpTo(enemyCharacter->GetActorRotation(), lookAt, 0.01f, enemyCharacter->rotateSpeed);

	enemyCharacter->SetActorRotation(rotator.Quaternion());
	enemyController->SetFocalPoint(playerCharacter->GetActorLocation(), EAIFocusPriority::Move);

	return EBTNodeResult::Succeeded;
}

