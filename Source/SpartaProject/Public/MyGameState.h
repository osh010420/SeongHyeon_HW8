// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MyGameState.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAPROJECT_API AMyGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	AMyGameState();
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	int32 score;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coin")
	int32 SpawnedCoin;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 CollectedCoin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	float LevelDuration;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentLevel;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 MaxLevel;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> LevelMapNames;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	UDataTable* WaveDataTable;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 CurrentWave;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 MaxWave;
	
	FTimerHandle WaveTimer;
	FTimerHandle LevelTimer;
	FTimerHandle HUDUpdateTimer;
	
	UFUNCTION(BlueprintPure, Category="Score")
	int32 GetScore() const;
	UFUNCTION(BlueprintCallable, Category="Score")
	void AddScore(int32 Amount);
	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();
	
	void StartLevel();
	void OnLevelTimeUp();
	void OnCoinCollected();
	void EndLevel();
	void UpdateHUD();
	
	void StartWave();
	void EndWave();
	void OnWaveTimeUP();
};
