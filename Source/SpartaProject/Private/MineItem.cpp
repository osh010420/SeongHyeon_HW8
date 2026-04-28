// Fill out your copyright notice in the Description page of Project Settings.


#include "MineItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AMineItem::AMineItem()
{
	ItemType = "MineItem";
	
	DestroyDamage = 30.0f;
	DestroyDelay = 5.0f;
	DestroyRadius = 300.0f;
	
	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	ExplosionCollision->InitSphereRadius(DestroyRadius);
	ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExplosionCollision->SetupAttachment(Scene);
}

void AMineItem::ActivateItem(AActor* Activator)
{
	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &AMineItem::Explode, DestroyDelay);
}

void AMineItem::Explode()
{
	TArray<AActor*> DestroyActors;
	ExplosionCollision->GetOverlappingActors(DestroyActors);
	
	for (AActor* Actor : DestroyActors)
	{
		if (Actor && Actor->ActorHasTag("Player"))
		{
			UGameplayStatics::ApplyDamage(
				Actor,
				DestroyDamage,
				nullptr,
				this,
				UDamageType::StaticClass());
		}
	}
	DestroyItem();
}
