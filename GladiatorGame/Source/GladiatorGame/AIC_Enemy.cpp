// Fill out your copyright notice in the Description page of Project Settings.


#include "AIC_Enemy.h"
#include "EnemyCharacter.h"
#include "LifeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "AIEnemyManager.h"
#include "EngineUtils.h"

AAIC_Enemy::AAIC_Enemy(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> obj(TEXT("/Game/Blueprints/Enemy/AI/BT_Enemy"));
	if (obj.Succeeded())
	{
		btree = obj.Object;
	}

	behaviorTreeComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorTreeComponent"));
	blackboard = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComponent"));

	SetActorTickEnabled(true);
}

void AAIC_Enemy::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(btree);
	behaviorTreeComponent->StartTree(*btree);
	
	blackboard->SetValueAsObject("PlayerActor", UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	FindAIEnemyManager();

}

void AAIC_Enemy::FindAIEnemyManager()
{
	for (TActorIterator<AAIEnemyManager> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AAIEnemyManager* aiEnemyManagerCast = Cast<AAIEnemyManager>(*ActorItr);
		if (aiEnemyManagerCast)
		{
			UE_LOG(LogTemp, Warning, TEXT("Enemy Manager found!"));
			aiEnemyManager = aiEnemyManagerCast;
			aiEnemyManager->AddEnemy(this);
			return;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Enemy Manager not found!"));
}

void AAIC_Enemy::OnPossess(APawn* const pawn)
{
	Super::OnPossess(pawn);
}

void AAIC_Enemy::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
}

void AAIC_Enemy::LaunchAttack()
{
	blackboard->SetValueAsEnum("MovingState", 6);
}

void AAIC_Enemy::AttackTerminated()
{
	blackboard->SetValueAsEnum("MovingState", 0);
	aiEnemyManager->AttackTerminated();
}


UBlackboardComponent* AAIC_Enemy::GetBB() const
{
	return blackboard;
}

