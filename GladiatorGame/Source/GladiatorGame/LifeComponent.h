// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LifeComponent.generated.h"


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

	DECLARE_DELEGATE(HurtDelegate);
	HurtDelegate OnHurt;

	DECLARE_DELEGATE(KillDelegate);
	KillDelegate OnKill;

};
