// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawnRow.h"
#include "SpawnVolume.generated.h"

class UBoxComponent;
UCLASS()
class SPARTAPROJECT_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* SpawningBox;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UDataTable* ItemDataTable;
	
	UFUNCTION(BlueprintCallable)
	FVector GetRandomPointInVolume() const;

	UFUNCTION(BlueprintCallable)
	void SpawnItem(TSubclassOf<AActor> ItemClass);
	
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnRandomItem();
	
	FItemSpawnRow* GetRandomItem() const;
};
