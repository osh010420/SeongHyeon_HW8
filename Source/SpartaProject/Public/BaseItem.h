// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyInterface.h"
#include "BaseItem.generated.h"

class USphereComponent;
UCLASS()
class SPARTAPROJECT_API ABaseItem : public AActor, public IMyInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemType;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* Collision;
	

	virtual void OnItemOverlap(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult) override;

	virtual void OnItemEndOverlap(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex) override;
	virtual void ActivateItem(AActor* Activator);
	virtual FName GetItemType() const;
	
	virtual void DestroyItem();

};
