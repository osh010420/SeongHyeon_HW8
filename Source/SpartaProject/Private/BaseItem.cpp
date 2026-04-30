// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItem.h"
#include "Components/SphereComponent.h"
#include "kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ABaseItem::ABaseItem()
{
 	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);
	
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->SetupAttachment(Scene);
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Collision);

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnItemOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnItemEndOverlap);
}

void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseItem::OnItemOverlap(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		GEngine->AddOnScreenDebugMessage(
			-1, 
			2.0f,
			FColor::Green, 
			FString::Printf(TEXT("Overlap!!!")));
	
		ActivateItem(OtherActor);
	}
}
void ABaseItem::OnItemEndOverlap(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex)
{
	
}
void ABaseItem::ActivateItem(AActor* Activator)
{
	UParticleSystemComponent* Particle = nullptr;
	if (ParticleSystem)
	{
		 Particle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ParticleSystem,
			GetActorLocation(),
			GetActorRotation(),
			true);
	}
	if (Sound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			Sound,
			GetActorLocation());
	}
	if (Particle)
	{
		FTimerHandle ParticleTimer;
		TWeakObjectPtr<UParticleSystemComponent> ParticleComponent = Particle;
		
		GetWorld()->GetTimerManager().SetTimer(
			ParticleTimer,
			[ParticleComponent]()
			{
				if (ParticleComponent.IsValid())
				{
					ParticleComponent->DestroyComponent();
				}
			},
			2.0f,
			false);
	}
}

FName ABaseItem::GetItemType() const
{
	return ItemType;
}

void ABaseItem::DestroyItem()
{
	Destroy();
}

