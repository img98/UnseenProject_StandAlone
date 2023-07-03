// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/HordeEnemy.h"
#include "AI/SAAIController.h"

AHordeEnemy::AHordeEnemy()
{
	AIControllerClass = ASAAIController::StaticClass();

	PrimaryActorTick.bCanEverTick = false; //Tick을 제거해 성능향상을 노림
}

void AHordeEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHordeEnemy::BeginPlay()
{
	Super::BeginPlay();
}
