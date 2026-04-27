// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "BaseCoin.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAPROJECT_API ABaseCoin : public ABaseItem
{
	GENERATED_BODY()
	
public:
	ABaseCoin();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PointValue;
};
