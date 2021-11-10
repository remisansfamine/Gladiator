// Fill out your copyright notice in the Description page of Project Settings.


#include "LifeComponent.h"

// Sets default values for this component's properties
ULifeComponent::ULifeComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void ULifeComponent::Hurt(int damage) 
{ 
	if (isInvicible)
		return;

	SetLife(life - damage);

	if (OnHurt.IsBound())
		OnHurt.Broadcast();

	if (invicibleCooldown <= 0.f)
		return;

	isInvicible = true;
	GetWorld()->GetTimerManager().SetTimer(invicibleTimer, this, &ULifeComponent::ResetInvicibility, invicibleCooldown, false);
}

void ULifeComponent::ResetInvicibility()
{
	isInvicible = false;
	GetWorld()->GetTimerManager().ClearTimer(invicibleTimer);

	if (OnInvicibilityStop.IsBound())
		OnInvicibilityStop.Broadcast();
}

void ULifeComponent::Kill()
{
	if (OnKill.IsBound())
		OnKill.Broadcast();
}
 
void ULifeComponent::SetLife(int value)
{
	life = value;

	if (OnLifeChanged.IsBound())
		OnLifeChanged.Broadcast(life);

	if (life <= 0)
		Kill();
}

