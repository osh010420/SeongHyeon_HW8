// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYPROJECT_API IMyInterface
{
	GENERATED_BODY()

public:
	virtual void OverlapItem(AActor* Actor) = 0;
	virtual void EndOverlapItem(AActor* Actor) =0;
	virtual void ActivateItem(AActor* Activator)=0;
	virtual FName GetItemType() const = 0;
};
