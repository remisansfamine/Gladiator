// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorInterComponent.h"

// Sets default values for this component's properties
UActorInterComponent::UActorInterComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

// Called when the game starts
void UActorInterComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UActorInterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}