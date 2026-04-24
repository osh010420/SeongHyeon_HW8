// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItem.h"


// Sets default values
ABaseItem::ABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ABaseItem::OverlapItem(AActor* Actor)
{
	
}
void ABaseItem::EndOverlapItem(AActor* OverlapActor)
{
	// 기본은 빈 함수 - 필요하다면 자식 클래스에서 활용
}


void ABaseItem::ActivateItem(AActor* Activator)
{
	// 기본은 빈 함수 - 자식 클래스에서 구현
}


FName ABaseItem::GetItemType() const
{
	return ItemType;
}


void ABaseItem::DestroyItem()
{

	Destroy();
}