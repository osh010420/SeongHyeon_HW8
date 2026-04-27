// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "MineItem.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAPROJECT_API AMineItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	AMineItem();
	
	virtual void ActivateItem(AActor* Activator) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DestroyDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DestroyRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DestroyDamage;
};
