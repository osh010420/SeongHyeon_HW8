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
	LevelDuration = 10.0f;
	CurrentLevel = 0;
	MaxLevel = 3;
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
	
	UpdateHUD();
	
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
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimer);
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
			}
		}
	}
}