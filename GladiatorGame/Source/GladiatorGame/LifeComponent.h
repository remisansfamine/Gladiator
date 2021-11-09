// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LifeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLifeDelegate);

UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class GLADIATORGAME_API ULifeComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Life, meta = (AllowPrivateAccess = "true"))
	int life;

	FTimerHandle invicibleTimer;


	bool isInvicible;

	UFUNCTION()
	void ResetInvicibility();

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Life, meta = (AllowPrivateAccess = "true"))
	float invicibleCooldown;

	// Sets default values for this component's properties
	ULifeComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable)
	void Hurt(int damage);
	
	UFUNCTION(BlueprintCallable)
	void Kill();

	void SetLife(int value);
	int GetLife() { return life; }

public:	
	UPROPERTY(BlueprintAssignable, Category = "Components|Life")
	FLifeDelegate OnInvicibilityStop;

	UPROPERTY(BlueprintAssignable, Category="Components|Life")
	FLifeDelegate OnHurt;

	UPROPERTY(BlueprintAssignable, Category = "Components|Life")
	FLifeDelegate OnKill;
};
