// Fill out your copyright notice in the Description page of Project Settings.


#include "GladiatorGameState.h"


AGladiatorGameState::AGladiatorGameState()
{
	OnKillPlayer.AddDynamic(this, &AGladiatorGameState::Defeat);
	OnKillEnemy.AddDynamic(this, &AGladiatorGameState::OnEnemyDeath);
}

void AGladiatorGameState::Defeat()
{
	if (OnGameTerminate.IsBound())
		OnGameTerminate.Broadcast(false);

}

void AGladiatorGameState::Victory()
{
	if (OnGameTerminate.IsBound())
		OnGameTerminate.Broadcast(true);
}

void AGladiatorGameState::OnEnemyDeath()
{
	enemiesCount--;
	if (enemiesCount <= 0)
		Victory();
}
