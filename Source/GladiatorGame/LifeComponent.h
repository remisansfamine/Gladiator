// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LifeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHurtDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKillDelegate);

UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class GLADIATORGAME_API ULifeComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraSettings, meta = (AllowPrivateAccess = "true"))
	int life;

public:	
	// Sets default values for this component's properties
	ULifeComponent();

	UFUNCTION()
	void Hurt(int damage);
	
	UFUNCTION()
	void Kill();

	void SetLife(int value);
	int GetLife() { return life; }

public:	

	UPROPERTY(BlueprintAssignable, Category="Components|Life")
	FHurtDelegate OnHurt;

	UPROPERTY(BlueprintAssignable, Category = "Components|Life")
	FKillDelegate OnKill;

	
};
