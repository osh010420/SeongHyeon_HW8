// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCoin.h"
#include "BigCoin.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAPROJECT_API ABigCoin : public ABaseCoin
{
	GENERATED_BODY()
	
public:
	ABigCoin();
	
	virtual void ActivateItem(AActor* Activator) override;
};
