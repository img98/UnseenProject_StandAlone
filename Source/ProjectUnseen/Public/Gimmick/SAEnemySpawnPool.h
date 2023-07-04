// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SAEnemySpawnPool.generated.h"


UCLASS()
class PROJECTUNSEEN_API ASAEnemySpawnPool : public AActor
{
	GENERATED_BODY()

public:	
	ASAEnemySpawnPool();

protected:
	virtual void BeginPlay() override;

	void Spawn();
	void IncreasePoolSize();

	UPROPERTY(EditAnywhere)
	class UBoxComponent* SpawnField;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AEnemyCharacter> SpawnEnemyClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASAAIController> SpawnAIControllerClass;
	UPROPERTY()
	FTimerHandle SpawnTimerHandle;
	UPROPERTY()
	float SpawnDelay = 2.f;

	UPROPERTY(VisibleAnywhere)
	TArray<AEnemyCharacter*> SpawningPool;
	UPROPERTY()
	int32 MaxPoolPopulation = 5;
	UPROPERTY()
	int32 IncreaseRate = 2;

public:	
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE TArray<AEnemyCharacter*> GetSpawningPool() { return SpawningPool; }
		
};
