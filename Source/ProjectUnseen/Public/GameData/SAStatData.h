// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SAStatData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUNSEEN_API USAStatData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	USAStatData();

	UPROPERTY(EditAnywhere, Category = Stat)
	float MaxHP;
	UPROPERTY(EditAnywhere, Category = Stat)
	float CurrentHP;
	UPROPERTY(EditAnywhere, Category = Stat)
	float AttackDamage;
	UPROPERTY(EditAnywhere, Category = Stat)
	float AttackSpeed;
};
