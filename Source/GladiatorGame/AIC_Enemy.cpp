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
}

void AAIC_Enemy::OnPossess(APawn* const pawn)
{
	Super::OnPossess(pawn);
}

void AAIC_Enemy::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	//blackboard->SetValueAsString("Debug", TEXT("Life = ") + FString::FromInt(character->lifeComponent->GetLife()));
}


UBlackboardComponent* AAIC_Enemy::GetBB() const
{
	return blackboard;
}

