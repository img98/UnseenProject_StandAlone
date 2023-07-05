// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/HordeEnemy.h"
#include "AI/SAAIController.h"

AHordeEnemy::AHordeEnemy()
{
	/** AIControllerClass�Ҵ��� ������Ƽ�� ���ִ���. �̰� Bp���� �Ҵ�������ұ� �ƴ� header���� �Ҵ��ؼ� �־���ұ�?*/
	AIControllerClass = ASAAIController::StaticClass();
}

void AHordeEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHordeEnemy::BeginPlay()
{
	Super::BeginPlay();
}
