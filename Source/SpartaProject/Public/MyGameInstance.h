// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAPROJECT_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UMyGameInstance();
	
	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category="GameData")
	int32 TotalScore;
	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category="GameData")
	int32 CurrentLevelIndex;
	
	UFUNCTION(BlueprintCallable, Category="GameData")
	void AddToScore(int32 Amount);
};
