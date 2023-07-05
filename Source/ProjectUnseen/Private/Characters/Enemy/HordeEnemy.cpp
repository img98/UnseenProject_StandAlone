// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/HordeEnemy.h"
#include "AI/SAAIController.h"

AHordeEnemy::AHordeEnemy()
{
	AIControllerClass = ASAAIController::StaticClass();

	PrimaryActorTick.bCanEverTick = false; //Tick�� ������ ��������� �븲
}

void AHordeEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHordeEnemy::BeginPlay()
{
	Super::BeginPlay();
}
