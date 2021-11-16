// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterState.generated.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8 {
	IDLE		UMETA(DisplayName = "Idle"),
	ATTACKING	UMETA(DisplayName = "Attacking"),
	DEFENDING	UMETA(DisplayName = "Defending"),
	DEAD	UMETA(DisplayName = "Dead")
};

