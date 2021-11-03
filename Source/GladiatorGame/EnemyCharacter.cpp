// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "LifeComponent.h"
#include "Components/CapsuleComponent.h"
#include "PlayerCharacter.h"

AEnemyCharacter::AEnemyCharacter()
	: AGladiatorGameCharacter()
{
	lifeComponent->SetLife(3);

}

void AEnemyCharacter::OnHit()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Arrete de me toucher man"));
}


