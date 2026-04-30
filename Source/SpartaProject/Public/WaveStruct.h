// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WaveStruct.generated.h"

USTRUCT(BlueprintType)
struct FWaveStruct : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float WaveDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 ItemSpawnCount;
};