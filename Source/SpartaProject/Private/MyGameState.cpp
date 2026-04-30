// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameState.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"
#include "BaseCoin.h"
#include "MyGameInstance.h"
#include "MyPlayerController.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "WaveStruct.h"
#include "BaseItem.h"

AMyGameState::AMyGameState()
{
	score = 0;
	SpawnedCoin = 0;
	CollectedCoin = 0;
	LevelDuration = 10.0f;
	CurrentLevel = 0;
	MaxLevel = 3;
	
	CurrentWave = 0;
	MaxWave = 0;
}

void AMyGameState::BeginPlay()
{
	Super::BeginPlay();

	StartLevel();
	
	GetWorldTimerManager().SetTimer(
			HUDUpdateTimer,
			this,
			&AMyGameState::UpdateHUD,
			0.1f,
			true
		);
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
	if (APlayerController* Controller = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(Controller))
		{
			MyPlayerController->ShowGameHUD();
		}
	}
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
		if (MyGameInstance)
		{
			CurrentLevel = MyGameInstance->CurrentLevelIndex;
		}
	}
    
	// 웨이브 데이터 테이블에서 MaxWave 가져오기
	if (WaveDataTable)
	{
		TArray<FWaveStruct*> AllWaveStructs;
		static const FString ContextString(TEXT("WaveContext"));
		WaveDataTable->GetAllRows(ContextString, AllWaveStructs);
		MaxWave = AllWaveStructs.Num();
	}
    
	// 첫 웨이브 시작 (아이템 스폰은 StartWave에서!)
	CurrentWave = 0;
	StartWave();
    
	UE_LOG(LogTemp, Warning, TEXT("Level %d Start!"), CurrentLevel + 1);
}

void AMyGameState::OnLevelTimeUp()
{
	if (CurrentWave>MaxWave)
	{
		EndLevel();
	}
}

void AMyGameState::OnCoinCollected()
{
	CollectedCoin++;
    
	UE_LOG(LogTemp, Warning, TEXT("Coin Collected: %d / %d"), 
		CollectedCoin,
		SpawnedCoin);
	
	if (SpawnedCoin > 0 && CollectedCoin >= SpawnedCoin)
	{
		EndWave();
	}
}

void AMyGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimer);
	GetWorldTimerManager().ClearTimer(WaveTimer);
	
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
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			MyPlayerController->SetPause(true);
			MyPlayerController->ShowMainMenu(true);
		}
	}
}

void AMyGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController);
		{
			if (UUserWidget* HUDWidget = MyPlayerController->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Timer"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(WaveTimer);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}
				
				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
						if (MyGameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), MyGameInstance->TotalScore)));
						}
					}
				}
				
				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevel + 1)));
				}
				
				if (UTextBlock* WaveIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Wave"))))
				{
					WaveIndexText->SetText(FText::FromString(FString::Printf(TEXT("Wave %d"), CurrentWave + 1)));
				}
			}
		}
	}
}

void AMyGameState::StartWave()
{
	if (!WaveDataTable) return;
    
	// 웨이브 데이터 가져오기
	TArray<FWaveStruct*> AllWaveStructs;
	static const FString ContextString(TEXT("WaveContext"));
	WaveDataTable->GetAllRows(ContextString, AllWaveStructs);
    
	if (!AllWaveStructs.IsValidIndex(CurrentWave)) return;
    
	//  현재 웨이브 데이터 꺼내기
	FWaveStruct* CurrentWaveData = AllWaveStructs[CurrentWave];
	if (!CurrentWaveData) return;
    
	// 코인 카운트 초기화
	SpawnedCoin = 0;
	CollectedCoin = 0;
    
	// SpawnVolume 찾기
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(),
		ASpawnVolume::StaticClass(),
		FoundVolumes);
    
	//  웨이브 데이터의 ItemSpawnCount만큼 스폰
	const int32 VolumeCount = CurrentWaveData->ItemSpawnCount;
    
	for (int32 i = 0; i < VolumeCount; ++i)
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
    
	//  웨이브 데이터의 시간으로 타이머 시작
	GetWorldTimerManager().SetTimer(
		WaveTimer,
		this,
		&AMyGameState::OnWaveTimeUP,
		CurrentWaveData->WaveDuration,  // ← 웨이브 시간 사용
		false);
    
	UpdateHUD();
    
	// 웨이브 시작 알림
	UE_LOG(LogTemp, Warning, TEXT("Wave %d Start! Items: %d, Duration: %.1f"),
		CurrentWave + 1,
		VolumeCount,
		CurrentWaveData->WaveDuration);
    
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			3.0f,
			FColor::Yellow,
			FString::Printf(TEXT("Wave %d Start!"), CurrentWave + 1));
	}
}

void AMyGameState::OnWaveTimeUP()
{
	EndWave();
}

void AMyGameState::EndWave()
{
	
	GetWorldTimerManager().ClearTimer(WaveTimer);
	
	TArray<AActor*> RemainingItems;
	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(),
		ABaseItem::StaticClass(),  
		RemainingItems);
    
	for (AActor* Item : RemainingItems)
	{
		if (Item)
		{
			Item->Destroy();
		}
	}
	
	CurrentWave++;
    
	
	if (CurrentWave >= MaxWave)
	{
		EndLevel();
		return;
	}
	
	StartWave();
}

