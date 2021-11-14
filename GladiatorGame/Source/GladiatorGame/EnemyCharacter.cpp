// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "LifeComponent.h"
#include "Components/CapsuleComponent.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"


AEnemyCharacter::AEnemyCharacter()
	: AGladiatorGameCharacter()
{
	healthComponent->SetLife(3);
	healthComponent->invicibleCooldown = 0.5f;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	playerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass()));
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!playerCharacter)
		return;

	//Move(playerCharacter->GetActorLocation() - GetActorLocation(), 500.f);
}
