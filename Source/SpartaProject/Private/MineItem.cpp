// Fill out your copyright notice in the Description page of Project Settings.


#include "MineItem.h"

AMineItem::AMineItem()
{
	ItemType = "MineItem";
	
	DestroyDamage = 30.0f;
	DestroyDelay = 5.0f;
	DestroyRadius = 300.0f;
}

void AMineItem::ActivateItem(AActor* Activator)
{
	
	DestroyItem();
}
