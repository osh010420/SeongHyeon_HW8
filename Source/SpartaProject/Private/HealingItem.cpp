// Fill out your copyright notice in the Description page of Project Settings.


#include "HealingItem.h"
#include "MyCharacter.h"

AHealingItem::AHealingItem()
{
	HealAmount = 20.0f;
	ItemType = "HealingItem";
}

void AHealingItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	if (Activator && Activator->ActorHasTag("Player"))
	{
		// 회복 디버그 메시지
		if (AMyCharacter* MyCharacter = Cast<AMyCharacter>(Activator))
		{
			MyCharacter->AddHealth(HealAmount);
		}
		DestroyItem();
	}
}

