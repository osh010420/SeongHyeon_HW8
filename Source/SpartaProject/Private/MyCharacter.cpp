// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "MyPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true; 
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;  
	
	NormalSpeed = 600.0f;
	SprintMultiplier = 1.5f;
	SprintSpeed = NormalSpeed * SprintMultiplier;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			if (PlayerController -> MoveAction)
			{
				EnhancedInput -> BindAction(
					PlayerController -> MoveAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::Move
				);
			}
			if (PlayerController -> LookAction)
			{
				EnhancedInput -> BindAction(
					PlayerController -> LookAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::Look
				);
			}
			if (PlayerController -> JumpAction)
			{
				EnhancedInput -> BindAction(
					PlayerController -> JumpAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::StartJump
				);
			}
			if (PlayerController -> JumpAction)
			{
				EnhancedInput -> BindAction(
					PlayerController -> JumpAction,
					ETriggerEvent::Completed,
					this,
					&AMyCharacter::StopJump
				);
			}
			if (PlayerController -> SprintAction)
			{
				EnhancedInput -> BindAction(
					PlayerController -> SprintAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::StartSprint
				);
			}
			if (PlayerController -> SprintAction)
			{
				EnhancedInput -> BindAction(
					PlayerController -> SprintAction,
					ETriggerEvent::Completed,
					this,
					&AMyCharacter::StopSprint
				);
			}
		}
	}

}

void AMyCharacter::Move(const FInputActionValue& value)
{
	//컨트롤러가 있어야 방향 계산이 가능하다.
	if (!Controller) return;
	
	const FVector2D MoveInput = value.Get<FVector2D>();
	// X축 정면으로, 반대는 Native로 후방으로
	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}	
	
	// 우측, 반대는 Native를 통해 좌측
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}

void AMyCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();
	
	AddControllerYawInput(LookInput.X);
	
	AddControllerPitchInput(LookInput.Y);
}

void AMyCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();
	}
}

void AMyCharacter::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();
	}
}

void AMyCharacter::StartSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void AMyCharacter::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}