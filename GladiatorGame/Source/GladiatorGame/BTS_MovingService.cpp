// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_MovingService.h"
#include "EnemyCharacter.h"
#include "PlayerCharacter.h"
#include "AIC_Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTS_MovingService::UBTS_MovingService(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("Moving Service");
}

void UBTS_MovingService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AEnemyCharacter* enemyCharacter = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	const APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("PlayerActor"));

	float distance = FVector::Dist(enemyCharacter->GetActorLocation(), playerCharacter->GetActorLocation());
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat("Distance", distance);

	OwnerComp.GetBlackboardComponent()->SetValueAsFloat("DeltaTime", DeltaSeconds);

	//UE_LOG(LogTemp, Warning, TEXT("CharacterMovement Activate"));

	static int oldEnumId;
	int enumId = OwnerComp.GetBlackboardComponent()->GetValueAsEnum("MovingState");

	if (enumId != oldEnumId)
	{
		if (enumId == 5 || enumId == 4)
		{
			enemyCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
			enemyCharacter->bUseControllerRotationYaw = false;
		}
		else
		{
			enemyCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
			enemyCharacter->bUseControllerRotationYaw = true;
		}

		oldEnumId = enumId;
	}
}