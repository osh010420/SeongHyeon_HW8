// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "MyGameState.h"
#include "Blueprint/UserWidget.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

AMyPlayerController::AMyPlayerController()
	: InputMappingContext(nullptr),
	  MoveAction(nullptr),
	  JumpAction(nullptr),
	  LookAction(nullptr),
	  SprintAction(nullptr),
      HUDWidgetClass(nullptr),
	  HUDWidgetInstance(nullptr),
	  MainMenuWidgetClass(nullptr),
	  MainMenuWidgetInstance(nullptr)
{
}

// 상단에 #include "EnhancedInputSubsystems.h" 선언
void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
				// 0 = 우선순위 (낮을수록 높은 우선순위)
			}
		}
	}
	
	FString CurrentLevel = GetWorld()->GetMapName();
	if (CurrentLevel.Contains("MenuLevel"))
	{
		ShowMainMenu(false);
	}
}


UUserWidget* AMyPlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

void AMyPlayerController::ShowMainMenu(bool bIsRestart)
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}
	
	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();
			
			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
		if (UTextBlock* ButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("StartButtonText"))))
		{
			if (bIsRestart)
			{
				ButtonText->SetText(FText::FromString(TEXT("Restart")));
			}
			else
			{
				ButtonText->SetText(FText::FromString(TEXT("Start")));
			}
		}
		if (bIsRestart)
		{
			UFunction* PlayAnimFunc = MainMenuWidgetInstance->FindFunction(FName("PlayGameOverAnim"));
			if (PlayAnimFunc)
			{
				MainMenuWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
			}
			if (UTextBlock* TotalScore = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("TotalScore"))))
			{
				if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this)))
				{
					TotalScore->SetText(FText::FromString(
						FString::Printf(TEXT("TotalScore : %d"), GameInstance->TotalScore)
						));
				}
			}
		}	
	}
}
void AMyPlayerController::ShowGameHUD()
{
	// HUD가 켜져 있다면 닫기
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}
	
	// 이미 메뉴가 떠 있으면 제거
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();
			
			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
			
			AMyGameState* MyGameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr;
			if (MyGameState)
			{
				MyGameState->UpdateHUD();
			}
		}
	}
}
void AMyPlayerController::StartGame()
{
	if (UMyGameInstance* SpartaGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		SpartaGameInstance->CurrentLevelIndex = 0;
		SpartaGameInstance->TotalScore = 0;
	}
	
	UGameplayStatics::OpenLevel(GetWorld(), FName("BasicLevel"));
}

