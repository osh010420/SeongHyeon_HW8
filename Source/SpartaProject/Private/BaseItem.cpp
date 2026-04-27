// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItem.h"



// Sets default values
ABaseItem::ABaseItem()
{
 	

}
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseItem::OverapItem(AActor* Actor)
{
	
}
void ABaseItem::EndOverapItem(AActor* Actor)
{
	
}
void ABaseItem::ActivateItem(AActor* Activator)
{
	
}
FName ABaseItem::GetItemType() const
{
	return ItemType;
}

void ABaseItem::DestroyItem()
{
	Destroy();
}

