// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameState.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"
#include "BaseCoin.h"
#include "MyGameInstance.h"
#include "MyPlayerController.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

AMyGameState::AMyGameState()
{
	score = 0;
	SpawnedCoin = 0;
	CollectedCoin = 0;
	LevelDuration = 30.0f;
	CurrentLevel = 0;
	MaxLevel = 3;
}

void AMyGameState::BeginPlay()
{
	Super::BeginPlay();
	
	// 게임 시작 시 첫 레벨부터 진행
	StartLevel();
}


void AMyGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
		if (MyGameInstance)
		{
			MyGameInstance->AddToScore(Amount);
		}
	}
}

int32 AMyGameState::GetScore() const
{
	return score;
}

void AMyGameState::StartLevel()
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
		if (MyGameInstance)
		{
			CurrentLevel = MyGameInstance->CurrentLevelIndex;
		}
	}

	SpawnedCoin = 0;
	CollectedCoin = 0;
	
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(),
		ASpawnVolume::StaticClass(),
		FoundVolumes);
	
	const int32 VolumeCount = 40;
	
	for (int32 i = 0; i<VolumeCount; ++i)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnedVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnedVolume)
			{
				AActor* SpawnActor = SpawnedVolume->SpawnRandomItem();
				if (SpawnActor && SpawnActor->IsA(ABaseCoin::StaticClass()))
				{
					SpawnedCoin++;
				}
			}
		}
	}
	GetWorldTimerManager().SetTimer(
		LevelTimer,
		this,
		&AMyGameState::OnLevelTimeUp,
		LevelDuration,
		false);
	
	UE_LOG(LogTemp, Warning, TEXT("Level %d Start!, Spawned %d coin"),
			CurrentLevel + 1,
			SpawnedCoin);
}

void AMyGameState::OnLevelTimeUp()
{
	EndLevel();
}

void AMyGameState::OnCoinCollected()
{
	CollectedCoin++;
	
	UE_LOG(LogTemp, Warning, TEXT("Coin Collected: %d / %d"), 
			CollectedCoin,
			SpawnedCoin)
	
	if (SpawnedCoin > 0 && CollectedCoin >= SpawnedCoin)
	{
		EndLevel();
	}
}

void AMyGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimer);
	
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
		if (MyGameInstance)
		{
			AddScore(score);
			CurrentLevel++;
			MyGameInstance->CurrentLevelIndex = CurrentLevel;
		}
	}

	if (CurrentLevel > MaxLevel)
	{
		OnGameOver();
		return;
	}
	
	if (LevelMapNames.IsValidIndex(CurrentLevel))
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevel]);
	}
	else
	{
		OnGameOver();
	}
}

void AMyGameState::OnGameOver()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Over!!"));
}