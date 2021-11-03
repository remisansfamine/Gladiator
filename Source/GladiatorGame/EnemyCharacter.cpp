// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "LifeComponent.h"
#include "Components/CapsuleComponent.h"
#include "PlayerCharacter.h"

AEnemyCharacter::AEnemyCharacter()
	: AGladiatorGameCharacter()
{
	lifeComponent->SetLife(5);
	capsuleComponent = GetCapsuleComponent();
	capsuleComponent->OnComponentHit.AddDynamic(this, &AEnemyCharacter::OnHit);

}

void AEnemyCharacter::OnHit(UPrimitiveComponent* hitComponent, AActor* otherActor, UPrimitiveComponent* otherComp,
	FVector normalImpulse, const FHitResult& hit)
{
	APlayerCharacter* character = Cast<APlayerCharacter>(otherActor);
	if (character)
	{
		character->lifeComponent->Hurt(1);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Je t'ai touche man"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Je t'ai pas touche man"));
	}

}


