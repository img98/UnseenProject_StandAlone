// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SAGameMode.generated.h"

DECLARE_MULTICAST_DELEGATE(FInitialSpawnSignature);
DECLARE_MULTICAST_DELEGATE(FIncreasePoolSizeSignature)

UCLASS()
class PROJECTUNSEEN_API ASAGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	
public:
	ASAGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void BeginPlay() override;

protected:
	UPROPERTY()
	FTimerHandle InitialSpawnTimer;
	UFUNCTION()
	void InitialSpawn(); //콜백형태로 각SpawningPool들과 연결
	UPROPERTY()
	FTimerHandle IncreasePoolSizeTimer;
	UFUNCTION()
	void IncreasePoolSize();

public:
	FInitialSpawnSignature OnInitialSpawn;
	FIncreasePoolSizeSignature OnIncreasePoolSize;
};
