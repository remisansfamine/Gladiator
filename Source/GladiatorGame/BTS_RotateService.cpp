// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_RotateService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnemyCharacter.h"
#include "PlayerCharacter.h"
#include "AIC_Enemy.h"

UBTS_RotateService::UBTS_RotateService(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("Rotate Service");
}

void UBTS_RotateService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AEnemyCharacter* enemyCharacter = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	static int oldEnumId;
	int enumId = OwnerComp.GetBlackboardComponent()->GetValueAsEnum("MovingState");

	if (enumId != oldEnumId)
	{
		if (enumId >= 4 && enumId != 6)
		{
			enemyCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
			enemyCharacter->bUseControllerRotationYaw = false;
		}
		else
		{
			enemyCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
			enemyCharacter->bUseControllerRotationYaw = true;

			AAIC_Enemy* enemyController = Cast<AAIC_Enemy>(enemyCharacter->GetController());
			enemyController->ClearFocus(EAIFocusPriority::Move);
		}

		oldEnumId = enumId;
	}
}