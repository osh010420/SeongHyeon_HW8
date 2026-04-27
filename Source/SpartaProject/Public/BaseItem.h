// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyInterface.h"
#include "BaseItem.generated.h"

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
	
	virtual void OverapItem(AActor* Actor);
	virtual void EndOverapItem(AActor* Actor);
	virtual void ActivateItem(AActor* Activator);
	virtual FName GetItemType() const;
	
	virtual void DestroyItem();

};
