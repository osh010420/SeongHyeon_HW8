// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCoin.h"
#include "MyGameState.h"

ABaseCoin::ABaseCoin()
{
	PointValue = 0;
	ItemType="BaseCoin";
}

void ABaseCoin::ActivateItem(AActor* Activator)
{
	if (Activator && Activator->ActorHasTag("Player"))
	{
		// 점수 획득 디버그 메시지
		if (UWorld* world = GetWorld())
		{
			if (AMyGameState* GameState = world->GetGameState<AMyGameState>())
			{
				GameState->AddScore(PointValue);
				GameState->OnCoinCollected();
			}
		}
		DestroyItem();
	}
}
