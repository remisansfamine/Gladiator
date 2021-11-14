// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "LifeComponent.h"
#include "Components/CapsuleComponent.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "AIC_Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIEnemyManager.h"



AEnemyCharacter::AEnemyCharacter()
	: AGladiatorGameCharacter()
{
	healthComponent->SetLife(3);
	healthComponent->invicibleCooldown = 0.5f;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	lifeComponent->OnKill.AddDynamic(this, &AEnemyCharacter::OnDeathEnemy);
	playerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass()));
}

void AEnemyCharacter::OnDeathEnemy()
{
	AAIC_Enemy* enemyController = Cast<AAIC_Enemy>(GetController());

	if (enemyController)
	{
		int enumId = enemyController->GetBlackboardComponent()->GetValueAsEnum("MovingState");

		if (enumId == 6 || enumId == 7)
			enemyController->AttackTerminated();

		enemyController->GetBlackboardComponent()->SetValueAsEnum("MovingState", 8);

		enemyController->aiEnemyManager->DeleteEnemy(enemyController);
	}
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
