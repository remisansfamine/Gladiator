// Fill out your copyright notice in the Description page of Project Settings.


#include "LifeComponent.h"

// Sets default values for this component's properties
ULifeComponent::ULifeComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void ULifeComponent::Hurt(int damage) 
{ 
	SetLife(life - damage);
	
	if (OnHurt.IsBound())
		OnHurt.Broadcast();
}

void ULifeComponent::Kill()
{
	if (OnKill.IsBound())
		OnKill.Broadcast();
}
 
void ULifeComponent::SetLife(int value)
{
	life = value;

	if (life <= 0)
		Kill();
}

