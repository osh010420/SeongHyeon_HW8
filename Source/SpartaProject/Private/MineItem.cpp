// Fill out your copyright notice in the Description page of Project Settings.


#include "MineItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AMineItem::AMineItem()
{
	ItemType = "MineItem";
	
	DestroyDamage = 100.0f;
	DestroyDelay = 5.0f;
	DestroyRadius = 300.0f;
	bHasExploded = false;
	
	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	ExplosionCollision->InitSphereRadius(DestroyRadius);
	ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExplosionCollision->SetupAttachment(Scene);
}

void AMineItem::ActivateItem(AActor* Activator)
{
	if (bHasExploded) return;
	Super::ActivateItem(Activator);
	
	GetWorld()->GetTimerManager().SetTimer(
		DestroyTimerHandle, 
		this,
		&AMineItem::Explode, 
		DestroyDelay);
	
	bHasExploded = true;
}

void AMineItem::Explode()
{
	UParticleSystemComponent* Particle = nullptr;
	if (DestroyParticle)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			DestroyParticle,
			GetActorLocation(),
			GetActorRotation(),
			false
			);
	}
	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			ExplosionSound,
			GetActorLocation());
	}
	
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
	
	if (Particle)
	{
		FTimerHandle DestroyParticleTimer;
		TWeakObjectPtr<UParticleSystemComponent> WeakParticle = nullptr;
		
		GetWorld()->GetTimerManager().SetTimer(
			DestroyParticleTimer,
			[WeakParticle]()
			{
				if (WeakParticle.IsValid())
				{
					WeakParticle->DestroyComponent();
				}
			},
			2.0f,
			false);
	}
}
