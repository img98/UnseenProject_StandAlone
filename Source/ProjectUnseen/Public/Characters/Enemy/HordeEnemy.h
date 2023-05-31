// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/EnemyCharacter.h"
#include "HordeEnemy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUNSEEN_API AHordeEnemy : public AEnemyCharacter
{
	GENERATED_BODY()
	
public:
	AHordeEnemy();

	virtual void Tick(float DeltaTime) override;


protected:
	virtual void BeginPlay() override;

private:

};
